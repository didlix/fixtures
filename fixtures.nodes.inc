<?php

/**
 * Print a list of the machine names of all content types on site
 *     
 * @param $content_type
 *  optional machine name of a content type will result in printing
 *  details for that content type
 * @author Rachel Graves
 **/
function fixtures_drush_get_content_types($content_type = null) {   

  foreach (node_get_types() as $machine_name => $content_type_array) {
      if(null === $content_type) {  
        drush_print($machine_name);
      } else if($machine_name === $content_type) {
        $ct = content_types($content_type);
        return drush_print_r($ct);
      }
  } 

  return true;
  
} 
      

/**
 * Generate a template of a fixture to make my life easier.
 *
 * @todo remove param
 * @return void
 * @author Rachel Graves
 **/
function fixtures_drush_generate_fixture_template($content_type_machine_name)
{    
  $i = 0;
  foreach(node_get_types() as $machine_name => $friendly_name) {
    $i++;
    drush_print($i . ':');
       
    $ct = content_types($machine_name);
    drush_print('  content_type: ' . $machine_name);
    
    foreach($ct['fields'] AS $field_name => $field_keys) {
      drush_print('  ' . $field_name . ': ' . json_encode(fixtures_drush_field_template($field_keys)));
    }    
  }
}                     


/**
 * Create a node  from a fixture
 *
 * @return void
 * @author Rachel Graves
 **/      
function fixtures_drush_create_node(stdClass $fixture) { 

  if(true === fixtures_drush_validate_fixture($fixture)) {
    $node = new stdClass;
    $node->title = $fixture['title'];
    $node->type  = $fixture['content_type'];
    $node->body  = $fixture['body'];
                                                          
    $node_fields = array('title', 'body', 'content_type');
        
    foreach($fixture AS $field_name => $field_value) {    
      
      if(!in_array($field_name, $node_fields)) {                
        $field = fixtures_drush_get_field($fixture['content_type'], $field_name);            
        $value =  fixtures_drush_prepare_field($field, $field_value);
        $node->$field_name = $value;           
      }                                                                                      
    }
    
    $node = node_prepare($node);
    node_submit($node);
    node_save($node);
 
  } else {

    return drush_set_error('fixtures', 'fixture supplied to fixtures_drush_create_node is not a valid fixture'); 

  }

}

/**
 * Makes sure a fixture is valid against the content type
 * And somehow implemented everything the node content type needs
 * Gasp.
 *
 * @return void
 * @author Rachel Graves
 **/
function fixtures_drush_validate_fixture($fixture) {

  foreach(node_get_types('names') as $machine_name => $friendly_name) {
                  
    $content_type = content_types($machine_name);
                 
    // drush_print_r($content_type);
    // Get cck field names
    foreach($content_type['fields'] AS $field_name => $field_keys) {
      $schema[$machine_name]['fields'][] = $field_keys['field_name'];
    }
  }
  
  // @todo this is supposed to be checking the schema

  return true;
}    

/**
 * undocumented function
 *                                                
 * @param content_type_machine_name machine name of content type
 * @param field_machine_name machine name of field
 * @return string of field type
 * @author Rachel Graves
 **/
function fixtures_drush_get_field($content_type_machine_name, $field_machine_name) { 
  
  $content_type = content_types($content_type_machine_name);

  // Get cck field names
  foreach($content_type['fields'] AS $field_name => $field_keys) {
    if($field_keys['field_name'] === $field_machine_name) {
      $field = $field_keys;
    }
  }
  
  // @todo better detection of non-cck fields, somehow?
  if(NULL === $field) {
    $field = array('type' => $field_machine_name);
  }

  return $field;
}

/**
 * Call everything implmenting hook_fixtures_field_FIELD_TYPE_prepare
 * Allows contrib modules (mainly cck) to implement prepare functions 
 * if this module goes public.  
 * If the field is not in content_types, it searches for a field handler
 * matching the field name. (Boo for not using CCK fields. Boo).
 *                                                 
 * @param $field taken from content_types()               
 * @param $field_value the value of the field that needs to be prepared.  
 * @return Array of prepared field
 * @author Rachel Graves
 **/
function fixtures_drush_prepare_field($field, $field_value) {
          
  $args = array($field_value, $field);                   
     
  $file = dirname(__FILE__) . '/includes/fields/' . $field['type'] . '.field.inc';

  if(file_exists($file)) {

    require_once($file);    
    $module = 'fixtures';
    $function = $module . '_' . 'fixtures_field_'. $field['type'] . '_prepare';        
    return call_user_func_array($function, $args);
    
  } else {
  
    foreach (module_implements('fixtures_field_'. $field['type'] . '_prepare') as $module) {        
      $function = $module . '_' . 'fixtures_field_'. $field['type'] . '_prepare';
      return call_user_func_array($function, $args);
    }     
  }
  
}
