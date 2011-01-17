<?php

/**
 * @file
 * Example class to rebuild node access permissions on the fly.
 */

class BotDaemon extends DrushDaemon {
  /**
   * Start the process
   */
  protected function beginProcess() {
    parent::beginProcess();

    // prevent MySQL timeouts on slow channels.
    db_query('SET SESSION wait_timeout = %d', 24*60*60);

    require_once('Net/SmartIRC.php');
    global $irc; // allow it to be slurped by Drupal modules if need be.
    $bot = new drupal_wrapper(); // wrapper that integrates with Drupal hooks.
    $this->irc = $irc = new Net_SmartIRC(); // MmmmmmM. The IRC object itself. Magick happens here.

    // Net_SmartIRC::nreplycodes is set from $_GLOBALS, which is unreliable.
    $irc->nreplycodes = $SMARTIRC_nreplycodes;
    $debug = drush_get_context('DRUSH_DEBUG') || drush_get_context('DRUSH_VERBOSE') || variable_get('bot_debugging', 0);
    $irc->setDebug( $debug ? SMARTIRC_DEBUG_ALL : SMARTIRC_DEBUG_NONE );
    // the (boolean) here is required, as Net_SmartIRC doesn't respect a FAPI checkbox value of 1, only TRUE.
    $irc->setAutoReconnect((boolean) variable_get('bot_auto_reconnect', 1)); // reconnect to the server if disconnected.
    $irc->setAutoRetry((boolean) variable_get('bot_auto_retry', 1)); // retry if a server connection fails.
    $irc->setUseSockets((boolean) variable_get('bot_real_sockets', 1)); // socket_connect or fsockopen?
    $irc->setChannelSyncing(TRUE); // keep a list of joined users per channel.

    // send every message type the library supports to our wrapper class.
    // we can automate the creation of these actionhandlers, but not the
    // class methods below (only PHP 5 supports default methods easily).
    $irc_message_types = array(
      'UNKNOWN',    'CHANNEL', 'QUERY',    'CTCP',        'NOTICE',       'WHO',
      'JOIN',       'INVITE',  'ACTION',   'TOPICCHANGE', 'NICKCHANGE',   'KICK',
      'QUIT',       'LOGIN',   'INFO',     'LIST',        'NAME',         'MOTD',
      'MODECHANGE', 'PART',    'ERROR',    'BANLIST',     'TOPIC',        'NONRELEVANT',
      'WHOIS',      'WHOWAS',  'USERMODE', 'CHANNELMODE', 'CTCP_REQUEST', 'CTCP_REPLY',
    );

    foreach ($irc_message_types as $irc_message_type) {
      $irc->registerActionhandler(constant('SMARTIRC_TYPE_' . $irc_message_type), '.*', $bot, 'invoke_irc_msg_' . strtolower($irc_message_type));
    }

    // set up a timers similar to Drupal's hook_cron(), multiple types. I would have
    // liked to just pass a parameter to a single function, but SmartIRC can't do that.
    $irc->registerTimehandler(300000, $bot, 'invoke_irc_bot_cron');           // 5 minutes.
    $irc->registerTimehandler(60000, $bot,  'invoke_irc_bot_cron_faster');    // 1 minute.
    $irc->registerTimehandler(15000, $bot,  'invoke_irc_bot_cron_fastest');   // 15 seconds.

    // connect and begin listening.
    $irc->connect(variable_get('bot_server', 'irc.freenode.net'), variable_get('bot_server_port', 6667));
    $irc->login(variable_get('bot_nickname', 'bot_module'), variable_get('bot_nickname', 'bot_module') . ' :http://drupal.org/project/bot', 8, variable_get('bot_nickname', 'bot_module'), (variable_get('bot_password', '') != '') ? variable_get('bot_password', '') : NULL);
  }

  /**
   * The actual process.
   */
  protected function process() {
    $this->irc->listenOnce();

    // Only return FALSE if something happens that should stop the daemon.
    return $this->irc->_state() == SMARTIRC_STATE_CONNECTED;
  }

  /**
   * End the process.
   */
  protected function endProcess() {
    $this->irc->disconnect();
    parent::endProcess();
  }

  /**
   * Check to see if hibernation is necessary.
   */
  protected function needsProcessing() {
    // Never hibernate.
    return TRUE;
  }
}

