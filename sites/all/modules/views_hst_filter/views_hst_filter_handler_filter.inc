<?php
/**
 * Filter by term id with hierarchical select.
 *
 */
class views_hst_filter_handler_filter extends views_handler_filter {

	// it's our filter unique??
	var $is_alone = FALSE;

	function init(&$view, &$options) {
		parent::init($view, $options);
		$this->operator = 'IN';
		$this->value = array('All');

	if (!empty($this->definition['vid'])) {
		$this->options['vid'] = $this->definition['vid'];
	}
}


	/**
	* Display the filter on the administrative summary
	*/
	function admin_summary() {
		$vocabulary = taxonomy_vocabulary_load($this->options['vid']);
		return t('Vocabulary in use:  ') . $vocabulary->name;
	}


	function option_definition() {
		$options = parent::option_definition();
		$options['exposed'] = array('default' => TRUE);
		$options['expose'] = array();
		$options['vid'] = array('default' => 0);
    $options['expose_label'] = array('default' => '');
    $options['nodes_counter'] = array('default' => 0);
    $options['empty_terms'] = array('default' => 0);
		return $options;
	}

	function options_form(&$form, &$form_state) {
		views_handler::options_form($form, $form_state);
		$form['clear_markup_start'] = array(
		'#markup' => '<div class="clearfix">',
		);
		// Add the subform from value_form().
		$this->show_value_form($form, $form_state);
		$form['clear_markup_end'] = array(
		'#markup' => '</div>',
		);

		$this->show_expose_form($form, $form_state);
	}


	/**
	* Options form subform for exposed filter options.
	*
	* @see options_form().
	*/
	function expose_form(&$form, &$form_state) {
		$form['#theme'] = 'views_ui_expose_filter_form';

	$form['label'] = array(
		'#type' => 'textfield',
		'#default_value' => $this->options['expose_label'],
		'#title' => t('Label'),
		'#size' => 40,
	);

	$vocabularies = taxonomy_get_vocabularies();
	$options = array();
	foreach ($vocabularies as $voc) {
		$options[$voc->vid] = check_plain($voc->name);
	}

	if (empty($this->options['vid'])) {
		$first_vocabulary = reset($vocabularies);
		$this->options['vid'] = $first_vocabulary->vid;
	}

    if (empty($this->definition['vid'])) {
		$form['vid'] = array(
			'#type' => 'radios',
			'#title' => t('Vocabulary'),
			'#options' => $options,
			'#default_value' => $this->options['vid'],
		);

		$form['nodes_counter'] = array(
			'#type' => 'checkbox',
				'#title' => t('Add nodes counter to each term'),
				'#default_value' => $this->options['nodes_counter'],
		);


		$form['empty_terms'] = array(
			'#type' => 'checkbox',
				'#title' => t('Hide empty terms'),
				'#default_value' => $this->options['empty_terms'],
		);

		/*$form['content_type'] = array(
			'#type' => 'select',
			'#title' => t('Content types to filter'),
			'#options' => array(
				'[any]' => t('any'),
				) + node_type_get_names()
			,
			'#default_value' => $this->options['content_type'],
		);
		*/

	}
}
	/**
	* Save options
	*/
	function expose_submit($form, &$form_state) {
		$this->options['expose_label'] = $form_state['input']['options']['label'];
		$this->options['nodes_counter'] = $form_state['input']['options']['nodes_counter'];
		$this->options['empty_terms'] = $form_state['input']['options']['empty_terms'];
	}

	function exposed_info() { return; }

	/**
	* Render our chunk of the exposed filter form when selecting
	* You can override this if it doesn't do what you expect.
	*/
	function exposed_form(&$form, &$form_state) {
		$vid = $this->options['vid'];
		$nodes_counter_option = $this->options['nodes_counter'];
		$empty_terms_option = $this->options['empty_terms'];

    $terms = $this->view->name.'_'.$this->view->current_display.'_'.$vid;
		$form[$terms] = array(
			'#type' => 'container',
			'#tree' => TRUE,
			'#prefix' => '<div id="' . $terms . '" class="container-inline">',
			'#suffix' => '</div>',
			'#name' => $terms,
		);

		if (isset($form_state['values'][$terms])) {
			$values = $form_state['values'][$terms];
		}
		elseif (isset($_GET[$terms])) {
			$values = $_GET[$terms];
		}
		else {
			$values = array();
		}

		$any_label = variable_get('views_exposed_filter_any_label', 'new_any') == 'old_any' ? t('<Any>') : t('- Any -');

		$values += array('term_' . $vid . '_' . (count($values)+1) => 'All');

		$parent = 0;
		foreach ($values as $input_name => $tid) {
			if ($parent === 'none' || !($child_terms = taxonomy_get_tree($this->options['vid'], $parent, 1))) {
				break;
			}

		$options = array('All' => $any_label);

		foreach ($child_terms as $term) {

			$nodes_counter = term_nc($term->tid);
			if($empty_terms_option == '1'){
				if($nodes_counter > 0){
					if($nodes_counter_option == '1'){
						$options[$term->tid] = $term->name.' ('.$nodes_counter.')';
					} else  {
						$options[$term->tid] = $term->name;
					};
				}
			} else {
				if($nodes_counter_option == '1'){
					$options[$term->tid] = $term->name.' ('.$nodes_counter.')';
				} else  {
					$options[$term->tid] = $term->name;
				};
			}

			//$options[$term->tid] = $term->name;

		}
			if (isset($_GET[$terms][$input_name])) {
				$default_value = $_GET[$terms][$input_name];
			}
			else {
				$default_value = $tid;
			}

			$form[$terms][$input_name] = array(
				'#type' => 'select',
				'#options' => $options,
				'#default_value' => $default_value,
				'#vid' => $this->options['vid'],
				'#ajax' => array(
					'callback' => 'views_hst_filter_ajax_callback',
					'wrapper' => $terms,
					'progress' => array('type' => 'none', 'message' => ''),
				),
			);
     // dsm($form[$terms]);

			if (isset($form_state['values'][$terms][$input_name])) {
				$parent = $form_state['values'][$terms][$input_name];
				}
				elseif (isset($_GET[$terms][$input_name])) {
					$parent = $_GET[$terms][$input_name];
				}
				else {
					$parent = 'none';
				}
	}
	$form['#info']['filter-terms-' . $terms] = array(
		'operator' => '',
		'value' => $terms,
		'label' => check_plain($this->options['expose_label']),
	);
}

  function exposed_validate($form, &$form_state) {
	$last_tid = 0;
	$is_any = FALSE;
	$vid = $this->options['vid'];
	$terms = $form_state['view']->name.'_'.$form_state['view']->current_display.'_'.$vid;
	foreach ($form_state['input'][$terms] as $tid) {
		if ($this->is_valid_tid($tid)) {
			$last_tid = $tid;
		}
		else {
			$is_any = TRUE;
			break;
		}
	}

	$tids = array();
	if ($is_any) {
		if (1 == count($form_state['input'][$terms])) {
			$this->value = array('All');
			return;
		}

		$tids = $this->get_term_child($this->options['vid'], $last_tid);
    }
    else {
		$tids = array(end($form_state['input'][$terms]));
	}
	$this->value = $tids;
	$this->check_for_alone($form_state);
  }


	/**
	* Add this filter to the query.
	*
	* Due to the nature of fapi, the value and the operator have an unintended
	* level of indirection. You will find them in $this->operator
	* and $this->value respectively.
	*/
	function query() {
	$this->ensure_my_table();

		if (1 == count($this->value) && !$this->is_valid_tid($this->value[0])) {
			$this->query->set_distinct(TRUE);
		}
		else {
			if ($this->is_alone) {
			$this->query->add_where($this->options['group'], "$this->table_alias.tid", $this->value, $this->operator);
		}
		  else {
			$this->query->set_distinct(TRUE);
			$nids = db_select('taxonomy_index', 'ti')
				->fields('ti', array('nid'))
				->condition('ti.tid', $this->value, 'IN')
				->execute()
				->fetchCol();
			$this->query->add_where($this->options['group'], "$this->table_alias.nid", $nids, 'IN');
		  }
		}
	}


	function is_valid_tid($tid) {
		return (!is_null($tid) && 'All' != $tid);
	}


	function get_term_child($vid, $tid) {
		$tids = array();
		if (isset($tid)) {
			$child_tids = taxonomy_get_tree($this->options['vid'], $tid);
			foreach ($child_tids as $term) {
				$tids[] = $term->tid;
			}
		}
		return $tids;
	  }

	function check_for_alone($form_state) {
		$keys = array_keys($form_state['input']);
		$is_alone = TRUE;
		$filters = array();
		foreach ($keys as $key) {
			if (FALSE !== strpos($key, 'terms') || 'tid' == $key || 'term_node_tid_depth' == $key) {
				$filters[] = $key;
			}
		}

		$this->is_alone = (count($filters) > 1) ? FALSE : TRUE;
	}
}


/**
 * AJAX callback
 */
function views_hst_filter_ajax_callback($form, &$form_state) {
  $element = $form_state['triggering_element'];
	return $form[$form_state['view']->name.'_'.$form_state['view']->current_display.'_'.$vid . $element['#vid']];
}

/**
*	taxonomy_term_count_nodes
*	http://drupal.org/node/602240
*	tid
*	Term ID
*	@param child_count
*	TRUE - Also count all nodes in child terms (if they exists) - Default
*	FALSE - Count only nodes related to Term ID
*/
function term_nc($tid, $child_count = TRUE) {
	$tids = array($tid);

	if ($child_count){
	$tids = array_merge($tids, term_get_children_ids($tid));
	}

	global $language;
	$langs = array($language->language);
	$langs[] = 'und';


	$query = db_select('taxonomy_index', 't');
	$query->condition('tid', $tids, 'IN');
	$query->join('node', 'n', 't.nid = n.nid');
	$query->condition('n.status', 1, '=');
		//$query->condition('n.type', 'article', '=');
	$query->condition('n.language', $langs, 'IN');

	$count = $query->countQuery()->execute()->fetchField();
	return  $count;
}

/**
*	Retrieve ids of term children .
*	@param $tid
*	The term's ID.
*	@param $tids
*	An array where ids of term children will be added
*/
function term_get_children_ids($tid) {
	$children = taxonomy_get_children($tid);
	$tids=array();

	if (!empty($children)){
		foreach($children as $child) {
			$tids[] = $child->tid;
			$tids = array_merge($tids, term_get_children_ids($child->tid));
		}
	}
	return $tids;
}
