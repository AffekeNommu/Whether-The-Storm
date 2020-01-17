# Whether the storm?

Updated from previous so much I created a new repository. Hardware and website all changed considerably.

ESP8266 and AM2320 website and logging Using a LoLin NodeMCU ESP8266 module and the AM2320 to display temperature and humidity readings in a website and log readings every 15 minutes to a MySQL database in the cloud with a PHP/JS website to display.

Info links for libraries etc: Setting up ESP8266 with Arduino info is here: http://arduino-esp8266.readthedocs.io/en/latest/installing.html

The AJAX website design is here: http://www.esp8266.com/viewtopic.php?f=8&t=4307

NTP time library is here: https://github.com/PaulStoffregen/Time

The arduino code is commented but a brief description is that it starts and sets up an AJAX website to display temperatures, humidity and time stamps. It displays current minimum and maximum temperatures and humidities with their respective times.

Every 15 minutes it logs the readings and time in UTC to a MySQL database.

My installation's readings are available at http://imgood.ruok.org if you would like to see.

The website files are: index.php which is the start page. It displays today's readings from midnight in 15 minute intervals. The function.php does all the database handling.

Assumption is that you have the MySQL installation on your webserver.

Database create SQL is:
```
  CREATE TABLE `Readings` (
    `RKey` int(11) NOT NULL,
    `Temperature` int(11) NOT NULL,
    `Humidity` int(11) NOT NULL,
    `TImeStamp` datetime NOT NULL
  ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
  ALTER TABLE `Readings`
    ADD PRIMARY KEY (`RKey`);
  ALTER TABLE `Readings`
    MODIFY `RKey` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=160;
  COMMIT;
```
