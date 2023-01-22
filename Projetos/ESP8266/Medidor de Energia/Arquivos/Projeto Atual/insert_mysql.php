<?php
$codigo = filter_input(INPUT_GET, 'codigo'); //variaveis da firmware esp8266
$tensao = filter_input(INPUT_GET, 'tensao'); //variaveis da firmware esp8266
$corrente = filter_input(INPUT_GET, 'corrente'); //variaveis da firmware esp8266
$potencia = filter_input(INPUT_GET, 'potencia'); //variaveis da firmware esp8266
$potenciahora = filter_input(INPUT_GET, 'potenciahora'); //variaveis da firmware esp8266
$frequencia = filter_input(INPUT_GET, 'frequencia'); //variaveis da firmware esp8266
$fatordepotencia = filter_input(INPUT_GET, 'fatordepotencia'); //variaveis da firmware esp8266

//Arquivo no diretorio /var/www/html do ubuntu server

$servername = "localhost";
$username = "XXXXXXXXXX"; //usuario do banco
$password = "XXXXXXXXXX"; //senha do banco
$dbname = "XXXXXXXXXX"; // nome do banco
$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
  //Gravar log de erros
  die("Não foi possível estabelecer conexão com o BD: " . $conn->connect_error);
}
$sql = "INSERT INTO medidor_de_energia.equipamento (codigo, tensao, corrente, potencia, potenciahora, frequencia, fatordepotencia) VALUES ($codigo,$tensao,$corrente,$potencia,$potenciahora,$frequencia,$fatordepotencia)";

if (!$conn->query($sql)) {
  //Gravar log de erros
  die("Erro na gravação dos dados no BD");
}
$conn->close();
?>


