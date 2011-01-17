<?php

/**
 * Implementation of hook_drush_help().
 */
function botd_drush_help($section) {
  switch ($section) {
    case 'drush:botd':
      return dt('Control the bot daemon.');
  }
}

/**
 * Implementation of hook_drush_command().
 */
function botd_drush_command() {
  $items = array();

  $items['botd'] = array(
    'daemon' => array(
      'class' => 'BotDaemon',
      'file' => 'BotDaemon.inc',
      // file path is only necessary if your include is in a separate directory
      // than this file.
      //'file path' => dirname(__FILE__),
    ),

    // Since we have our own custom process method, you'll need to specify this
    // command-hook here, or Drush will try to find a callback function for this
    // command and whine at you about it.
    'command-hook' => 'drushd',

    // Everything below here is just typical drush command parameters.
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
    'description' => 'Control the bot daemon.',
    'drupal dependencies' => array('bot'),
    'drush depencencies' => array('drushd'),
    // Good to include these arguments, so people know how to use the daemon.
    'arguments' => array(
      'start'    => 'Start the daemon.',
      'status'   => 'Display status information about the daemon.',
      'stop'     => 'Stop the daemon.',
      'restart'  => 'Restart the daemon',
      'show-log' => 'Show the log file.',
    ),
    'examples' => array(
      'drush botd start'                         => 'Start the daemon.',
      'drush botd start --verbose'               => 'Log verbosely.',
      ' '   => '',
      'drush botd status' => 'Show the current status of the daemon.',
      '  '  => '',
      'drush botd stop'              => 'Stop the daemon, allowing 45 seconds for processing.',
      'drush botd stop --timeout=10' => 'Allow 10 seconds for processing.',
      'drush botd stop --queue'      => 'Queue the node access rebuild daemon to stop.',
      '   ' => '',
      'drush botd restart'              => 'Restart the daemon, allowing 45 seconds for processing.',
      'drush botd restart --timeout=10' => 'Allow 10 seconds for processing.',
      'drush botd restart --queue'      => 'Queue the node access rebuild daemon to restart.',
      '    ' => '',
      'drush botd show-log' => 'Show the log file, using less.',
      'drush botd show-log --watch' => 'Watch the log file.',
      'drush botd show-log --tail' => 'Show just the tail of the log file, to see recent messages.',
    ),
  );

  return $items;
}

// pass off IRC messages to our modules via Drupal's hook system.
class drupal_wrapper {
  function invoke_irc_bot_cron(&$irc)                 { module_invoke_all('irc_bot_cron'); }
  function invoke_irc_bot_cron_faster(&$irc)          { module_invoke_all('irc_bot_cron_faster'); }
  function invoke_irc_bot_cron_fastest(&$irc)         { module_invoke_all('irc_bot_cron_fastest'); }
  function invoke_irc_msg_unknown(&$irc, &$data)      { module_invoke_all('irc_msg_unknown', $data); }
  function invoke_irc_msg_channel(&$irc, &$data)      { module_invoke_all('irc_msg_channel', $data); }
  function invoke_irc_msg_query(&$irc, &$data)        { module_invoke_all('irc_msg_query', $data); }
  function invoke_irc_msg_ctcp(&$irc, &$data)         { module_invoke_all('irc_msg_ctcp', $data); }
  function invoke_irc_msg_notice(&$irc, &$data)       { module_invoke_all('irc_msg_notice', $data); }
  function invoke_irc_msg_who(&$irc, &$data)          { module_invoke_all('irc_msg_who', $data); }
  function invoke_irc_msg_join(&$irc, &$data)         { module_invoke_all('irc_msg_join', $data); }
  function invoke_irc_msg_invite(&$irc, &$data)       { module_invoke_all('irc_msg_invite', $data); }
  function invoke_irc_msg_action(&$irc, &$data)       { module_invoke_all('irc_msg_action', $data); }
  function invoke_irc_msg_topicchange(&$irc, &$data)  { module_invoke_all('irc_msg_topicchange', $data); }
  function invoke_irc_msg_nickchange(&$irc, &$data)   { module_invoke_all('irc_msg_nickchange', $data); }
  function invoke_irc_msg_kick(&$irc, &$data)         { module_invoke_all('irc_msg_kick', $data); }
  function invoke_irc_msg_quit(&$irc, &$data)         { module_invoke_all('irc_msg_quit', $data); }
  function invoke_irc_msg_login(&$irc, &$data)        { module_invoke_all('irc_msg_login', $data); }
  function invoke_irc_msg_info(&$irc, &$data)         { module_invoke_all('irc_msg_info', $data); }
  function invoke_irc_msg_list(&$irc, &$data)         { module_invoke_all('irc_msg_list', $data); }
  function invoke_irc_msg_name(&$irc, &$data)         { module_invoke_all('irc_msg_name', $data); }
  function invoke_irc_msg_motd(&$irc, &$data)         { module_invoke_all('irc_msg_motd', $data); }
  function invoke_irc_msg_modechange(&$irc, &$data)   { module_invoke_all('irc_msg_modechange', $data); }
  function invoke_irc_msg_part(&$irc, &$data)         { module_invoke_all('irc_msg_part', $data); }
  function invoke_irc_msg_error(&$irc, &$data)        { module_invoke_all('irc_msg_error', $data); }
  function invoke_irc_msg_banlist(&$irc, &$data)      { module_invoke_all('irc_msg_banlist', $data); }
  function invoke_irc_msg_topic(&$irc, &$data)        { module_invoke_all('irc_msg_topic', $data); }
  function invoke_irc_msg_nonrelevant(&$irc, &$data)  { module_invoke_all('irc_msg_nonrelevant', $data); }
  function invoke_irc_msg_whois(&$irc, &$data)        { module_invoke_all('irc_msg_whois', $data); }
  function invoke_irc_msg_whowas(&$irc, &$data)       { module_invoke_all('irc_msg_whowas', $data); }
  function invoke_irc_msg_usermode(&$irc, &$data)     { module_invoke_all('irc_msg_usermode', $data); }
  function invoke_irc_msg_channelmode(&$irc, &$data)  { module_invoke_all('irc_msg_channelmode', $data); }
  function invoke_irc_msg_ctcp_request(&$irc, &$data) { module_invoke_all('irc_msg_ctcp_request', $data); }
  function invoke_irc_msg_ctcp_reply(&$irc, &$data)   { module_invoke_all('irc_msg_ctcp_reply', $data); }
}
