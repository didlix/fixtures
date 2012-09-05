<?php

/**
 * Create a user from a fixture
 * 
 * @param Array
 *  Array of a user from a fixture
 * @return void
 * @author Rachel Graves
 **/
function fixtures_drush_create_user($fixture) {    

  $user = array();

  foreach($fixture AS $key => $value) {

    if('roles' === $key && is_array($value)) {
      $user['roles'] = fixtures_drush_get_role_ids($value);  
    } else {
      $user[$key] = $value;
    }
  }       

  if(!isset($user['init'])) {
    $user['init'] = $user['mail'];
  }
   
  drush_print_r($user);
  user_save(null, $user);
}     


/**
 * For given roles find their associated role IDS
 * @param role_names  array of role names
 * @return associative array of roles and their IDs
 */
function fixtures_drush_get_role_ids($role_names = array()) {
  //find all the roles in the system
  $roles = user_roles();
  foreach ((array)$role_names as $role) {
    //foreach of the role provided check it exists in teh system
    $fnd_role = array_search($role, $roles);
    if (is_numeric($fnd_role)) {
      //if an id is found add to the array to be returned
      $rids[$fnd_role] = $role;
    }
  }
  return $rids;
}