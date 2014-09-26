<?php 
//ADDED we need these variables due to custom requests
global $user;
$user_roles = $user->roles;
$the_url = request_uri();
$the_url=parse_url($the_url);
$arr = explode("/", $the_url['path']);
//END
//print_r($node);
if(isset($node)){
if($node->type=='group'){
?>
<style type='text/css'>
.project-<?php echo $node->nid;?> a{
	background-color: #EDF3FA !important;
}
.project-<?php echo $node->nid;?> ul li a{
	background-color: #CFE5FD !important;
}
</style>
<?php
}elseif($node->type=='collection'){
?>
<style type='text/css'>
.collection-<?php echo $node->nid;?>{
	background-color: #EDF3FA !important;
}
</style>
<?php	
}
}
 ?>
<div class="texture-overlay">
  <div id="page" class="container <?php print $classes; ?>">
    <?php print render($page['menu_bar']); // Menubar region ?>
    <header id="header" class="clearfix" role="banner">
      <div class="header-inner clearfix">

        <?php //REMOVING DEFAULT BRANDING. USING Delta Blocks Module for logo, slogan, etc.
		if ($site_logo || $site_name || $site_slogan): ?>
          <!-- start: Branding -->
          <div id="branding" class="branding-elements clearfix">

            <?php if ($site_logo): ?>
              <div id="logo">
                <?php print $site_logo; ?>
              </div>
            <?php endif; ?>

            <?php if ($site_name || $site_slogan): ?>
              <!-- start: Site name and Slogan hgroup -->
              <hgroup id="name-and-slogan"<?php print $hgroup_attributes; ?>>

                <?php if ($site_name): ?>
                  <h1 id="site-name"<?php print $site_name_attributes; ?>><?php print $site_name; ?></h1>
                <?php endif; ?>

                <?php if ($site_slogan): ?>
                  <h2 id="site-slogan"<?php print $site_slogan_attributes; ?>><?php print $site_slogan; ?></h2>
                <?php endif; ?>

              </hgroup><!-- /end #name-and-slogan -->
            <?php endif; ?>

          </div><!-- /end #branding -->
        <?php endif; ?>

        <?php print render($page['header']); // Header region ?>
<?php print render($page['secondary_content']); // Secondary content region (Secondary) ?>
      </div>

    </header> <!-- /header -->







    <!-- Three column 3x33 Gpanel -->
    <?php if (
      $page['three_33_top'] ||
      $page['three_33_first'] ||
      $page['three_33_second'] ||
      $page['three_33_third'] ||
      $page['three_33_bottom']
      ): ?>
      <div class="at-panel gpanel panel-display three-3x33 clearfix">
        <?php print render($page['three_33_top']); ?>
        <?php print render($page['three_33_first']); ?>
        <?php print render($page['three_33_second']); ?>
        <?php print render($page['three_33_third']); ?>
        <?php print render($page['three_33_bottom']); ?>
      </div>
    <?php endif; ?>

    <div id="columns">

      <div class="columns-inner clearfix">
<div id="extra-top-padding">
    <!-- Messages and Help -->
    <?php print $messages; ?>
    <?php print render($page['help']); ?>

    <!-- Breadcrumbs -->
    <?php 
//ADDED or rather removed because directors didnt like them. added back to news items
	if($arr[1]=='news' && isset($arr[2])){
	//if ($breadcrumb): print $breadcrumb; endif; 
	if(!$breadcrumb){
		$breadcrumb='<div id="breadcrumb" class="clearfix"><nav class="breadcrumb-wrapper clearfix" role="navigation"><ol id="crumbs" class="clearfix">
<li class="crumb crumb-first"><span typeof="v:Breadcrumb"><a href="/news">News</a></span></li><li class="crumb crumb-last"><span class="crumb-separator"> » </span><span class="crumb-title">'.$node->title.'</span></li></ol></nav></div>';
	}elseif (strpos($breadcrumb,'href="/') !== true) {
		$breadcrumb = str_replace('href="','href="/',$breadcrumb);
	}
	print $breadcrumb;
	}else{
		//print_r($arr);
	}
//END
	?>
        <div id="content-column">
          <div class="content-inner">

            <?php print render($page['highlight']); // Highlighted region ?>

            <<?php print $tag; ?> id="main-content" role="main">

              <?php print render($title_prefix); ?>
              <?php if ($title || $primary_local_tasks || $secondary_local_tasks || $action_links = render($action_links)): ?>
                <header id="main-content-header" class="clearfix">

                  <?php if ($title): ?>
                    <h1 id="page-title"><?php print $title; ?></h1>
                  <?php endif; ?>

<?php
//ADDED to put tabs in for admins and on themes, designers also
if (array_key_exists(2,$arr)) {
$second = $arr[2];
}else{
$second="";
}
if (array_key_exists(1,$arr)) {
$first = $arr[1];
}else{
$first="";
}
$last=end($arr);
$access_tabs=0;
if(($first=='user'||$first=='taxonomy') && $last=='edit'){
	$access_tabs=1;
}
if ($second=="appearance" && in_array("designer", $user_roles)){
	$access_tabs=1;
}
if ($first=="development-issue" || $second=="login"){
	$access_tabs=1;
}
if(($first=="documentation" || $first=="help" || $first=="news") && in_array("documentation group", $user_roles)){
	$access_tabs=1;
}
if (in_array("administrator", $user_roles) || $access_tabs==1){?>
                  <?php if ($primary_local_tasks || $secondary_local_tasks || $action_links): ?>
                    <div id="tasks" class="clearfix">

                      <?php if ($primary_local_tasks): ?>
                        <ul class="tabs primary clearfix"><?php print render($primary_local_tasks); ?></ul>
                      <?php endif; ?>

                      <?php if ($secondary_local_tasks): ?>
                        <ul class="tabs secondary clearfix"><?php print render($secondary_local_tasks); ?></ul>
                      <?php endif; ?>

                      <?php if ($action_links = render($action_links)): ?>
                        <ul class="action-links clearfix"><?php print $action_links; ?></ul>
                      <?php endif; ?>

                    </div>
                  <?php endif; ?>
<?php }
//END ADDED for tab access
//ADDED to restrict viewing of citation data due to ancient biblio module
if (!in_array("administrator", $user_roles) && ($first=='citation-data' || $first=='biblio')){
drupal_goto("");
}
//END
?>
                </header>
              <?php endif; ?>
              <?php print render($title_suffix); ?>
              <?php if ($content = render($page['content'])): ?>
                <div id="content">
                  <?php print $content; // Main content region ?>
                </div>
              <?php endif; ?>
              <?php
			  //ADDED an annoying theme hack to move the advanced search blocks
//			  $the_url = request_uri();
//			  if($the_url=="/search/site"){
				//print render($page['sidebar_second']);
//				print render($page['sidebar_first']);
//			  }
			  //END
              ?>

              <!-- Feed icons (RSS, Atom icons etc -->
              <?php print $feed_icons; ?>

            </<?php print $tag; ?>> <!-- /main-content -->

            <?php print render($page['content_aside']); // Aside region ?>

          </div>
        </div> <!-- /content-column -->

        <?php 
		//ADDED an annoying theme hack to move the advanced search blocks
//		$the_url = request_uri();
//		if($the_url!="/search/site"){
		//END
			print render($page['sidebar_first']); // Sidebar first region 
			print render($page['sidebar_second']); // Sidebar second region 
		//ADDED an annoying theme hack to move the advanced search blocks
//		}
		//END
		?>
      </div>
    </div> <!-- /columns -->

    <?php print render($page['tertiary_content']); // Tertiary content region (Tertiary) ?>

    <!-- four-4x25 Gpanel -->
    <?php if (
      $page['four_first'] ||
      $page['four_second'] ||
      $page['four_third'] ||
      $page['four_fourth']
      ): ?>
      <div class="at-panel gpanel panel-display four-4x25 clearfix">
        <div class="panel-row row-1 clearfix">
          <?php print render($page['four_first']); ?>
          <?php print render($page['four_second']); ?>
        </div>
        <div class="panel-row row-2 clearfix">
          <?php print render($page['four_third']); ?>
          <?php print render($page['four_fourth']); ?>
        </div>
      </div>
      </div>
    <?php endif; ?>

    <?php if ($page['footer']): ?>
      <footer id="footer" role="contentinfo">
        <div id="footer-inner" class="clearfix">
          <?php print render($page['footer']); // Footer region ?>
        </div>
      </footer>
    <?php endif; ?>

  </div> <!-- /page -->
</div> <!-- /texture overlay -->
