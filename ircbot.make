core = 7.x
api = 2

projects[drupal][version] = "7.0"
; Since this project has no node types, upgrades will require this patch.
; See http://drupal.org/node/1017672 & http://drupal.org/node/1133402
projects[drupal][patch][] = http://drupal.org/files/issues/node_types_D7.patch

projects[ircbot_profile][type] = "profile"
projects[ircbot_profile][download][type] = git
projects[ircbot_profile][download][url] = http://git.drupal.org/project/ircbot_profile.git
projects[ircbot_profile][download][branch] = 7.x-1.x

