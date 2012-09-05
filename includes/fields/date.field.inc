<?php
  
 /**
  * Implements hook_fixtures_prepare_field
  * Prepare location fields
  *
  * @param $data string of location i.e. GEOMETRYCOLLECTION(POINT(1.0626685 52.2972388))
  * @return array
  * @author Rachel Graves
  **/
function fixtures_fixtures_field_date_prepare($field_value, $field) {

  drush_print_r($field);

  $prepared_field = array();
  $date = array(
    'value'       =>  $field_value,
    'timezone'    =>  date_default_timezone_name(),
    'timezone_db' =>  date_default_timezone_name(), 
    'date_type'   =>  'date'
  );
  $prepared_field[] = $date;     

  return $prepared_field;  
}        

/**      
 * Implementes hook_fixtures_field_FIELD_TYPE_template
 *
 * @return a formatted example for a fixture's template
 * @todo take note of allowed_values, allowed_values_php,
 *  prefix, suffix, required, default_value, default_value_php  
 * @todo implement date stuff
 */
function fixtures_fixtures_field_date_template($field) {

  $template = array(    
    'type'          => $field['type'],
    'granularity'   => $field['granularity'],
    'input_format'  => $field['widget']['input_format'],
    );
    
  return $template;
  
}