Signature

Требуется написать консольную программу на C++ для генерации сигнатуры указанного файла.

Сигнатура генерируется следующим образом: исходный файл делится на блоки равной

(фиксированной) длины (если размер файла не кратен размеру блока, последний фрагмент может

быть меньше или дополнен нулями до размера полного блока). Для каждого блока вычисляется

значение hash функции и дописывается в выходной файл-сигнатуру.