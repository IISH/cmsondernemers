<?php
require_once "../config.inc.php";

global $COOKIE;

class class_authentication {

	public static function authenticate( $login, $password ) {
		return class_authentication::check_ldap('iisgnet\\' . $login, $password, array('sa-dc02.iisg.net'));
	}

	public static function check_ldap($user, $pw, $servers) {
		$login_correct = 0;

		// LDAP AUTHENTICATIE VIA PHP-LDAP
		// php-ldap moet geinstalleerd zijn op de server

		foreach ( $servers as $server ) {
			if ( $login_correct == 0 ) {

				// connect
				$ad = ldap_connect($server) or die ("Could not connect to $server. Please contact IT Servicedesk");

				// set some variables
				ldap_set_option($ad, LDAP_OPT_PROTOCOL_VERSION, 3);
				ldap_set_option($ad, LDAP_OPT_REFERRALS, 0);

				// bind to the ldap directory
				$bd = @ldap_bind($ad, $user, $pw);

				// verify binding
				if ($bd) {
					$login_correct = 1;
				}

				// never forget to unbind!
				ldap_unbind($ad);
			}
		}

		return $login_correct;
	}
}

// + + + + + +

$usernaam = "";
$userwachtwoord = "";
$inloggenMislukt = "0";
$verkeerdeRechten = "0";

// get eerste deel van servernaam
$servernaam = ( $_SERVER["HTTP_X_FORWARDED_HOST"] != '' ? $_SERVER["HTTP_X_FORWARDED_HOST"] : $_SERVER['SERVER_NAME'] );
$pos = strpos($servernaam, '.');
if ( $pos !== false ) {
	$servernaam = substr($servernaam, 0, $pos);
}
$servernaam = "'" . $servernaam . "'";

// is form gesubmit
$issubmitted = $_POST["issubmitted"];
if ( $issubmitted == "1" ) {
	// get login / wachtwoord
	$usernaam = trim($_POST['login']);
	$userwachtwoord = trim($_POST['pass']);

	// CHECK OF BEIDE WAARDES ZIJN INGEVULD
	if ( $usernaam == "" || $userwachtwoord == "" ) {
		$inloggenMislukt = "1";
	}

	// CHECK OF USER/WACHTWOORD CORRECT ZIJN
	if ( $inloggenMislukt == "0" && $verkeerdeRechten == "0" ) {
		$result_login_check = class_authentication::authenticate($usernaam, $userwachtwoord);

		if ( $result_login_check == 1 ) {
			setcookie("atkuser", $usernaam, time()+7200, '/');
		} else {
			$inloggenMislukt = "1";
		}
	}
	// CHECK OF USER DE PAGINA MAG ZIEN
	if ( $inloggenMislukt == "0" && $verkeerdeRechten == "0" ) {
		//
		if ( !in_array($usernaam, $allowedUsers) ) {
			$verkeerdeRechten = "1";
		}
	}
	// ZO JA, SAVE SESSION
	// EN REDIRECT NAAR PAGINA WAAR WE VANDAAG KWAMEN
	if ( $inloggenMislukt == "0"  && $verkeerdeRechten == "0" ) {
		if ( $_GET['ref'] != "" ) {
			$redirect = $_GET['ref'];
		} else {
			$redirect = "/";
		}

		//
		$redirect = str_replace("http://backend1.", "http://", $redirect);
		Header("Location: " . $redirect);
	}
	// ZO NEE, TOON INVULSCHERM EN TOON FOUTMELDING
	$userwachtwoord = "";
	if ( $inloggenMislukt == "1" ) {
		$inlogcode = "";
		setcookie("atkuser", "", time(), '/');
	} else {
		$inlogcode = $usernaam;
	}

} else {
	$issubmitted = "0";
	$inlogcode = $_COOKIE["atkuser"];
}

$url2script = $_SERVER['SCRIPT_NAME'];
if ( $_SERVER['QUERY_STRING'] != "" ) {
	$url2script .= "?" . $_SERVER['QUERY_STRING'];
}
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<html>
<head>
	<title><?php echo $servernaam; ?> inloggen</title>
	<meta http-equiv="content-type" content="text/html; charset=iso-8859-1">

	<link href="../atk/style.css" rel="stylesheet" type="text/css">
	<link href="../atk/themes/iisg/styles/style.css" rel="stylesheet" type="text/css">
	<link href="../atk/themes/default//styles/menu.css" rel="stylesheet" type="text/css">

</head>
<body bgcolor="#CCCCCC" onLoad="">
<center>
<table bgcolor="#708090" width="100%" align="center" border="0" cellpadding="2" cellspacing="1">
	<TR>
		<TD align="center" bgcolor="red"><font color="white" sizexxx="-1"><b>Inloggen verplicht...</b></font></TD>
	</TR>
	<tr>
		<!-- einde head -->
	<td bgcolor="#ffffff" width="100%" valign="top" colspan="2">
		<table width="100%" border="0" cellspacing="0" cellpadding="20">
		<tr>
			<td valign="top"><font face="Arial, Helvetica" size="+1" color="#000000"><b><?php echo $servernaam; ?> inloggen</b></font><br><br>
Bepaalde delen van <?php echo $servernaam; ?> zijn afgeschermd. Om toegang te krijgen tot deze gedeelten is het nodig om in te loggen. Vul op deze pagina je (normale) inlognaam en wachtwoord in.<p>

<?php if ( $inloggenMislukt == "1" ) { ?>
<font color=red><b>Inloggen mislukt.<br></b></font>Als het inloggen blijft mislukken stuur dan een mailtje naar <a href="mailto:helpdesk@iisg.nl">helpdesk@iisg.nl</a>.
<?php } elseif ( $verkeerdeRechten == "1" ) { ?>
<font color=red><b>U heeft de verkeerde rechten voor de door u gekozen website.<br></b></font>
<?php } ?>

<FORM NAME="passwordForm" action="<?php echo $url2script; ?>" method="POST">
<table>
<tr>
	<td>Login name &nbsp;</td>
	<td><input type="Text" name="login" size=8 tabindex=1 value="<?php echo $usernaam; ?>"></td>
</tr>
<tr>
	<td>Password &nbsp; &nbsp;</td>
	<td><input type="Password" name="pass" size=8 tabindex=2 value="<?php echo $userwachtwoord; ?>"></td>
</tr>
</table>
<input type="hidden" name="issubmitted" value="1">
<input type="Submit" value="Login" tabindex=3>
</form>
<script language="javascript">
     document.passwordForm.login.focus();
</script>
</form>
		</td>
		</tr>
		</table>
	</td>
</tr>
</table>
</center>
</body>
</html>
