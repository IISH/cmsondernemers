<?php
class mod_ondernemers extends atkModule
{
	function getMenuItems() {
		menuitem("ondernemers");

		menuitem("submenu_ondernemers", dispatch_url("ondernemers.ondernemers", "admin"), "ondernemers");
		menuitem("submenu_sbicodes", dispatch_url("ondernemers.sbicodes", "admin"), "ondernemers");

		menuitem("-", "", "ondernemers");
	}
}
