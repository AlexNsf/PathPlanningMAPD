#LifelongMAPD

Для запуска проекта требуется запустить main.cpp и первым аргументом
подать на вход программе xml-файл (примеры есть в папке xml_examples).
Далее нужно запустить visualizer.py, предварительно установив библиотеку
pygame.

Входной xml файл должен иметь следующий вид:
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<root>
    <map>
        <width>3</width>
        <height>3</height>
        <initial_coordinates>2 0</initial_coordinates>
        <start_coordinates>0 0</start_coordinates>
        <finish_coordinates>2 2</finish_coordinates>
        <tasks>0 0 0 0 0 0 0 0 0 0</tasks>
        <grid>
            <row>0 1 0</row>
            <row>0 0 0</row>
            <row>0 0 0</row>
        </grid>
    </map>
</root>
```
Теги width и height отвечают за ширину и толщину соответственно <br/>
Теги initial_coordinates, start_coordinates и finish_coordinates отвечают
за начальные координаты агентов, места приема и места выдачи соответственно.
Координаты подаются в формате [x, y] парами. <br/>
Тег tasks отвечает за задания, в формате [start_coordinate_num, finish_coordinate_num]
парами. <br/>
В тегах row построчно передается карта, где 0 - проходимая клетка, а
1 - непроходимая.