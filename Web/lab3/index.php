<!DOCTYPE html> 
<html> 
    <head> 
        <title>Тестируем PHP</title> 
    </head> 
    <body> 
<?php  
$a=strftime("%d %b %Y", time() );   
?> 
        <p>Привет, мир! Сегодня <?=$a?></p> 
    </body> 
</html> 