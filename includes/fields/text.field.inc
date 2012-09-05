<?php
  
 /**
  * Implements hook_fixtures_prepare_field
  * Prepare location fields
  *
  * @param $data string of location i.e. GEOMETRYCOLLECTION(POINT(1.0626685 52.2972388))
  * @return array
  * @author Rachel Graves
  **/
function fixtures_fixtures_field_text_prepare($field_value, $field) {
  $prepared_field = array();
  $prepared_field[]['value'] = $field_value;     
  
  return $prepared_field;  
}        

/**      
 * Implementes hook_fixtures_field_FIELD_TYPE_template
 *
 * @return Array
 *  
 * @todo take note of allowed_values, allowed_values_php,
 *  prefix, suffix, required, default_value, default_value_php
 */
function fixtures_fixtures_field_text_template($field) {
        // drush_print_r($field);
  $desc = array();
  $desc['type'] = $field['type'];
  
  if(1 === (int)$field['widget_active']) {
    $desc['label'] = $field['widget']['label'];
  }    
  
  $desc['required'] = $field['required'];
  $desc['max_length'] = $field['max_length'];  

  return $desc;
  
}