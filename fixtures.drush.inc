<?php
  
/**
 * Implementation of hook_drush_command()
 */
function fixtures_drush_command() {

  $items = array();
  $items['fixtures-generate-content'] = array(   
    'bootstrap'   => DRUSH_BOOTSTRAP_DRUPAL_FULL,  
    'callback'    => 'fixtures_drush_generate_content',
    'description' => dt('Generate nodes from YAML'),
    'examples'    => array(
      'drush fixgc path/to/fixtures.yaml' => 
      'Generate content from fixtures.yaml'),
    'arguments'   => array(
      'source' => 'Required. The location of the fixtures yaml'
    ),
    'aliases'     => array('fixgc')
  );  
$items['fixtures-generate-users'] = array(   
  'bootstrap'   => DRUSH_BOOTSTRAP_DRUPAL_FULL,  
  'callback'    => 'fixtures_drush_generate_users',
  'description' => dt('Generate users from YAML'),
  'examples'    => array(
    'drush fixgc path/to/fixtures.yaml' => 
    'Generate content from fixtures.yaml'),
  'arguments'   => array(
    'source' => 'Required. The location of the fixtures yaml'
  ),
  'aliases'     => array('fixgu')
);
  $items['fixtures-get-fields'] = array(   
    'bootstrap'   => DRUSH_BOOTSTRAP_DRUPAL_FULL,  
    'callback'    => 'fixtures_drush_get_fields',
    'description' => dt('Get the field names'),
    'examples'    => array(
      'drush fixgf blog' => 
      'Get field machine names from content type blog'),
    'arguments'   => array(
      'contenttype' => 'Required. The machine name of the content type'
    ),
    'aliases'     => array('fixgf')
  );    
$items['fixtures-generate-fixture-template'] = array(   
  'bootstrap'   => DRUSH_BOOTSTRAP_DRUPAL_FULL,  
  'callback'    => 'fixtures_drush_generate_fixture_template',
  'description' => dt('Generate fixutres template'),
  'examples'    => array(
    'drush fixgft blog' => 
    'Output a fixtures template for content type blog'),
  'arguments'   => array(
    'contenttype' => 'Required. The machine name of the content type'
  ),
  'aliases'     => array('fixgft')
);   
  $items['fixtures-get-content-types'] = array(   
    'bootstrap'   => DRUSH_BOOTSTRAP_DRUPAL_FULL,  
    'callback'    => 'fixtures_drush_get_content_types',
    'description' => dt('Get the field names'),
    'examples'    => array(
      'drush fixgct' => 
      'Get field machine names of all content types'),
    'aliases'     => array('fixgct')
  );

  $items['fixtures-make-me-some-fing-user-yaml'] = array(   
    'bootstrap'   => DRUSH_BOOTSTRAP_DRUPAL_FULL,  
    'callback'    => 'fixtures_drush_bake_user_yaml',
    'description' => dt('MAKE YAML'),
    'examples'    => array(
      'drush MAKEYAML uid' => 
      'Get field machine names of all content types'),
    'arguments'   => array(
      'contenttype' => 'Required. The machine name of the content type'
    ),      
    'aliases'     => array('BAKEUSERYAML')
  );
  
  return $items;                 
}                
  
/**
 * Generates node content from yaml
 *
 * @return void
 * @author Rachel Graves
 **/
function fixtures_drush_generate_users($path_to_yaml) {
    
  require_once('fixtures.users.inc');
            
  // Wrapped in an if, desp
  if($fixtures = fixtures_drush_parse_yaml($path_to_yaml)) {  
    foreach($fixtures AS $fixture) {
      fixtures_drush_create_user($fixture);
    }    
  }
  
}     


/**
 * drush_print_r some YAML NOM
 *
 * @return void
 * @author Rachel Graves
 **/
function fixtures_drush_bake_user_yaml($uid) {
  $user = user_load(array('uid' => $uid)); 
  $user->foo = array('a user');
  return fixtures_drush_bake_yaml((array)$user);
  
}  

/**
 * undocumented function
 *
 * @return void
 * @author Rachel Graves
 **/
function fixtures_drush_bake_yaml($data) {
  
  require_once( dirname(__FILE__) . '/lib/sfYaml.php' );
  return drush_print_r(sfYaml::dump($data));                      

}


/**
 * Implementation of hook_drush_help()   
 * @todo provide help
 */
function fixtures_drush_help() {}           

/**
 * Generates node content from yaml
 *
 * @return void
 * @author Rachel Graves
 **/
function fixtures_drush_generate_content($path_to_yaml) {
   
  require_once('fixtures.nodes.inc');
            
  // Wrapped in an if, desp
  if($fixtures = fixtures_drush_parse_yaml($path_to_yaml)) {  
    foreach($fixtures AS $fixture) {
      fixtures_drush_create_node($fixture);
    }    
  }
  
}                        

/**
 * Loads in the symfony yaml lib and parses yaml
 * http://components.symfony-project.org/yaml
 *
 * @return Array of yaml
 * @author Rachel Graves
 **/
function fixtures_drush_parse_yaml($path_to_yaml) {

  if(file_exists($path_to_yaml)) {
    require_once( dirname(__FILE__) . '/lib/sfYaml.php' );
    return sfYaml::load($path_to_yaml);                      
  } else {
    return drush_set_error('fixtures', 'supplied yaml file does not exist');
  }

}

/**
 * Output cck field machine names to the command line
 *
 * @todo make this work, it does not
 * @return void
 * @author Rachel Graves
 **/
function fixtures_drush_get_fields($content_type_machine_name) {
  $content_type = content_types($content_type_machine_name);

  // Get cck field names
  foreach($content_type['fields'] AS $field_name => $field_keys) {
    drush_print($field_name . ': ' . $field_keys['type']);
    // if($field_keys['field_name'] === $field_machine_name) {
    //   $field = $field_keys;
    // }                          
  }

  return true;
}