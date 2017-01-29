<?php
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
/*

        drive.php
        Drive IOT-Trike

        Driver software requests action with URI args:
        1. drive.php?action=LT // turn left
        2. drive.php?action=RT // turn right
        3. drive.php?action=FW // go forth
        4. drive.php?action=ST // stop
	5. drive.php?action=RV // reverse - not implemment
        No action is taken and one line is written to log.

        IOT-Trike software queries action by calling drive.php with no args.
        Action is returned on page e.g. LT, RT, FWD, STP.


*/

$action = (isset($_GET['action']) ? $_GET['action'] : null);
if($action == "") {
        $cmd = "tail -1 /var/log/apache2/access.log";
        $cmdout = shell_exec($cmd);
        $findme = "action=";
        $pos = strpos($cmdout, $findme);
        if($pos) {
                echo substr($cmdout, $pos + strlen($findme), 2);
        };

}
$origin = (isset($_GET['origin']) ? $_GET['origin'] : null);
if($origin == "browser") {
echo <<< END_OF_TEXT
<html>
 <body>
  <table align="center">
        <tr>
                <td colspan=3 align="center"><a href="drive.php?action=FW&origin=browser"><img src="images/up-arrow.png" alt="GO"></a>
                </td>
        </tr>
        <tr>
                <td><a href="drive.php?action=LT&origin=browser"><img src="images/left-arrow.png" alt="LEFT TURN"></a>
                </td>
                <td><a href="drive.php?action=ST&origin=browser"><img src="images/stop.png" alt="STOP"></a>
                </td>
                <td><a href="drive.php?action=RT&origin=browser"><img src="images/right-arrow.png" alt="RIGHT TURN"></a>
                </td>
        </tr>
        <tr>
                <td colspan=3 align="center"><a href="drive.php?action=RV&origin=browser"><img src="images/return-arrow.png" alt="REVERSE"></a>
                </td>
        </tr>
  </table>
 </body>
</html>
END_OF_TEXT;
}
?>
