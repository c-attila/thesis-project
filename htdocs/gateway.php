<?php

$servername = "localhost";
$username = "root";
$password = "";
$database = "arduino_database";

$conn = mysqli_connect($servername, $username);

    if (isset($_POST["value"]) || isset($_POST["time"])) {
        mysqli_select_db($conn, $database) or die(mysql_error());
        $sql = "";

        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        // echo "Connected successfully \n";
        // echo $_POST["value"] . " degrees" . "\n";

        $sql = "INSERT INTO records (value, time) VALUES (" . $_POST["value"] . ", FROM_UNIXTIME(" . $_POST["time"] . "));";
        if (mysqli_query($conn, $sql)) {
            // echo "Data added.\n---------------------------------\n";
        } else {
            echo "Error: " . $sql . "\n" . mysqli_error($conn);
        }
    }

    if (isset($_GET["range"])) {
        mysqli_select_db($conn, $database) or die(mysql_error());
        $sql = "";

        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        //echo "Connected successfully <br>";

        if ($_GET["range"] == "current") {
            $sql = "SELECT * FROM records
                        ORDER BY time DESC
                        LIMIT 1;";

            $result = mysqli_query($conn, $sql);

            if (mysqli_num_rows($result) > 0) {
                while($row = mysqli_fetch_assoc($result)) {
                    echo $row["value"] . " °C";
                }
            } else {
                echo "0 results";
            }
        }

        if ($_GET["range"] == "24h") {
            $sql = "SELECT ROUND(AVG(value), 2) AS value FROM records
                        WHERE time > DATE_SUB(NOW(), INTERVAL 1 DAY)";
            
            $avg_result = mysqli_query($conn, $sql);

            if (mysqli_num_rows($avg_result) > 0) {
                while($row = mysqli_fetch_assoc($avg_result)) {
                    echo "Average temperature: " . $row["value"] . " °C\n";
                }
            } else {
                echo "0 results";
            }
            echo "\n";

            $sql = "SELECT value, time FROM records
                        WHERE time > DATE_SUB(NOW(), INTERVAL 1 DAY)
                        AND value = (SELECT MAX(value) FROM records
                            WHERE time > DATE_SUB(NOW(), INTERVAL 1 DAY))";

            $max_result = mysqli_query($conn, $sql);

            if (mysqli_num_rows($max_result) > 0) {
                echo "Max temperature: ";
                while($row = mysqli_fetch_assoc($max_result)) {
                    echo $row["value"] . " °C at " . $row["time"] . "\n";
                }
            } else {
                echo "0 results";
            }
        }

        if ($_GET["range"] == "1week") {
            $sql = "SELECT ROUND(AVG(value), 2) AS value FROM records
                        WHERE time > DATE_SUB(NOW(), INTERVAL 1 WEEK)";
            
            $avg_result = mysqli_query($conn, $sql);

            if (mysqli_num_rows($avg_result) > 0) {
                while($row = mysqli_fetch_assoc($avg_result)) {
                    echo "Average temperature: " . $row["value"] . " °C\n";
                }
            } else {
                echo "0 results";
            }
            echo "\n";

            $sql = "SELECT value, time FROM records
                        WHERE time > DATE_SUB(NOW(), INTERVAL 1 WEEK)
                        AND value = (SELECT MAX(value) FROM records
                            WHERE time > DATE_SUB(NOW(), INTERVAL 1 WEEK))";

            $max_result = mysqli_query($conn, $sql);

            if (mysqli_num_rows($max_result) > 0) {
                echo "Max temperature: \n";
                while($row = mysqli_fetch_assoc($max_result)) {
                    echo $row["value"] . " °C at " . $row["time"] . "\n";
                }
            } else {
                echo "0 results";
            }
        }
    }

$conn->close(); 
?>