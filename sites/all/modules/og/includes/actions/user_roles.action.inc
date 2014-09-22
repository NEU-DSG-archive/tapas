<?php

function og_user_roles_action_info() {
//ADDED to unset $_SESSION below
	if(isset($_SESSION['userlabels'])){
		unset($_SESSION['userlabels']);
	}
//END
  return array('og_user_roles_action' => array(
    'type' => 'og_membership',
    'label' => t('Modify OG user roles'),
    'configurable' => TRUE,
  ));
}

//ADDED $form_state to show users being altered. see http://drupal.org/node/1293364
//function og_user_roles_action_form($context) {
function og_user_roles_action_form($context, &$form_state) {
//END
  $gid = '';
  $group_type = '';

  if (module_exists('og_context') && $og_context = og_context()) {
    // Get the group type and group ID from OG context.
    $gid = $og_context['gid'];
    $group_type = $og_context['group_type'];
  }
  elseif (empty($gid) || empty($group_type)) {
    // Get the group type and group ID from the Views arguments if they exist.
    list($group_type, $gid) = $context['view']->args;
    if (empty($group_type) || empty($gid)) {
      return;
    }
  }

  if (!entity_get_info($group_type)) {
    return;
  }

  $group = entity_load_single($group_type, $gid);
  if (!$group || !og_is_group($group_type, $group)) {
    return;
  }
  list(,, $bundle) = entity_extract_ids($group_type, $group);
  $og_roles = og_roles($group_type, $bundle, $gid, FALSE, FALSE);

  $form['add_roles'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Add roles'),
    '#description' => t('Choose one or more roles you would like to assign to the selected users.'),
    '#options' => $og_roles,
    '#size' => 5
  );
  $form['remove_roles'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Remove roles'),
    '#description' => t('Choose one or more roles you would like to remove from the selected users.'),
    '#options' => $og_roles,
    '#size' => 5
  );
//ADDED to generate a list of selected users to modify
  $count=0;
  unset($_SESSION['userlabels']);
foreach($context['view']->result as $i){
	$id=$i->uid;
	$og_membership=og_get_membership($group_type, $gid, 'user', $id);
	
	foreach($form_state['selection'] as $h){
		if($og_membership->id==$h){
			//echo $og_membership->id;
			//echo "break";
			$current_role=og_get_user_roles($group_type, $gid, $id);
			//echo '<pre>'; print_r($current_role); echo '</pre>';
			$j="";
			foreach($current_role as $k){
				if($j!=""){
					$j=$j."<li>".$k."</li>";
				}else{
					$j="<li>".$k."</li>";
				}
			}
			//exit();
			//$_SESSION is called in Block 5 - list of users selected
			$_SESSION['userlabels'][$count]="<td><a href='user/$id' target='_blank'>".og_membership_label($og_membership)."</a></td><td><ul>".$j."</ul></td>";
		}
	}
	$count++;
}
//END
  return $form;
}

function og_user_roles_action_validate($form, $form_state) {
  if (!$form_state['values']['add_roles'] && !$form_state['values']['remove_roles']) {
    form_set_error('add_roles', t('You have not chosen any role to add or remove. Please select something to do.'));
  }
}

function og_user_roles_action_submit($form, $form_state) {
//ADDED to unset $_SESSION above
unset($_SESSION['userlabels']);
//END
  return array(
    'add_roles' => array_filter($form_state['values']['add_roles']),
    'remove_roles' => array_filter($form_state['values']['remove_roles']),
  );
}

function og_user_roles_action($og_membership, $context) {
  if ($og_membership->entity_type != 'user') {
    return;
  }

  $group_type = $og_membership->group_type;
  $gid = $og_membership->gid;

  foreach ($context['add_roles'] as $rid) {
    og_role_grant($group_type, $gid, $og_membership->etid, $rid);
  }

  foreach ($context['remove_roles'] as $rid) {
    og_role_revoke($group_type, $gid, $og_membership->etid, $rid);
  }
}
