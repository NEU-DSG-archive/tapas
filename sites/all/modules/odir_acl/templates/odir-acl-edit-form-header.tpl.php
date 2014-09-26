<?php
/**
 * @file
 * Header of permission editing form.
 */
?>
<div id="odir-acl-edit-form-header">
<h2><?php echo t('Permissions for')?> "<b><?php echo $role ?></b>" <?php echo t('to directory')?> "<b><?php echo $path ?></b>"</h2>
<p><b><?php echo t('Note')?></b>: <?php echo t('Permissions will be inherited by subfolders if they are not overidden')?>.</p>
<p><?php echo t('Permissions inherit from global "Directory based organisational layer" permissions')?>, <?php echo t('see')?>: <?php print l(t("Drupal permissions"), "admin/people/permissions")?>.</p>
</div>
