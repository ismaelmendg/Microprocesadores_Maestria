# from PIL import Image
#
# def image_to_c_array(image_path, array_name):
#
#     img = Image.open(image_path)
#
#
#     img = img.convert('RGBA')
#
#     img = img.resize((128, 160))
#
#
#     pixel_matrix = list(img.getdata())
#
#
#     hex_matrix = [f"0x{((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3):04X}" for (r, g, b, a) in pixel_matrix]
#
#
#     hex_matrix = [hex_matrix[i:i + 128] for i in range(0, len(hex_matrix), 128)]
#
#
#     print(f"const uint16_t {array_name}[160][128] = {{")
#     for row in hex_matrix:
#         print("{" + ",".join(row) + "},")
#     print("};")
#
#
# original_image_path = 'IMG.TIFF'
#
#
# output_image_path = 'output_image.bmp'
#
# array_name = 'test_img_128x160'
#
#
# image_to_c_array(original_image_path, array_name)

# from PIL import Image
#
# # Cargar la imagen de 16 bits a color
# imagen_16bits_color = Image.open("P.png")
#
# # Obtener los píxeles de la imagen
# pixels = list(imagen_16bits_color.getdata())
#
# # Ancho y alto de la imagen
# width, height = imagen_16bits_color.size
#
# # Crear una lista para almacenar los elementos del array en el formato específico
# array_lista = []
#
# # Iterar sobre los píxeles y agregarlos al array_lista
# for y in range(height):
#     row = "{"
#     for x in range(width):
#         pixel = pixels[y * width + x]
#         row += "0x{:04X},".format(pixel[0])  # Usar pixel[0] para acceder al valor en lugar de pixel directamente
#     row = row[:-1] + "},"
#     array_lista.append(row)
#
# # Crear el array resultante con el formato específico
# result_array = "{" + "\n".join(array_lista) + "};"
#
# # Imprimir el array resultante
# print(result_array)

# from PIL import Image
#
# def image_to_c_array(image_path, array_name):
#     img = Image.open(image_path)
#     img = img.convert('RGBA')
#     img = img.resize((128, 160))
#
#     pixel_matrix = list(img.getdata())
#
#     hex_matrix = [
#         f"0x{((min(r, 255) >> 3) << 11) | ((min(g, 255) >> 2) << 5) | (min(b, 255) >> 3):04X}"
#         for (r, g, b, a) in pixel_matrix
#     ]
#
#     hex_matrix = [hex_matrix[i:i + 128] for i in range(0, len(hex_matrix), 128)]
#
#     print(f"const uint16_t {array_name}[160][128] = {{")
#     for row in hex_matrix:
#         print("{" + ",".join(row) + "},")
#     print("};")
#
#
#
# original_image_path = 'P.png'
#
#
# output_image_path = 'output_image.bmp'
#
# array_name = 'test_img_128x160'
#
#
# image_to_c_array(original_image_path, array_name)
######################################################################################################################
#GENERAR MATRIZ CASI CORRECTA
# from PIL import Image
#
# def image_to_c_array(image_path, array_name):
#     img = Image.open(image_path)
#     img = img.convert('RGB')  # Cambiado a modo RGB (sin canal alfa)
#     img = img.resize((128, 160))
#
#     pixel_matrix = list(img.getdata())
#
#     # Ajustar el orden de los canales según la interpretación de tu TFT
#     hex_matrix = [
#         f"0x{((min(b, 255) >> 3) << 11) | ((min(r, 255) >> 2) << 5) | (min(g, 255) >> 3):04X}"
#         for (r, g, b) in pixel_matrix  # No se utiliza el canal alfa en esta línea
#     ]
#
#     hex_matrix = [hex_matrix[i:i + 128] for i in range(0, len(hex_matrix), 128)]
#
#     print(f"const uint16_t {array_name}[160][128] = {{")
#     for row in hex_matrix:
#         print("{" + ",".join(row) + "},")
#     print("};")
#
# # Ruta de la imagen original
# original_image_path = 'P.bmp'
#
# # Ruta de la imagen de salida (puedes ajustarla según tus necesidades)
# output_image_path = 'output_image.bmp'
#
# # Nombre del array en formato C
# array_name = 'test_img_128x160'
#
# # Llamada a la función
# image_to_c_array(original_image_path, array_name)


########################################################################################################################
#IMPRIMIR ALTERNATIVAS BGR
#
# import pygame
# import numpy as np
#
# # Ruta al archivo que contiene la matriz
# ruta_archivo = 'matriz_limpia.txt'
#
# # Leer la matriz desde el archivo
# with open(ruta_archivo, 'r') as file:
#     lines = file.readlines()
#
# # Filtrar solo las líneas que contienen valores de la matriz
# lineas_matriz = [line for line in lines if 'const uint16_t' not in line]
#
# # Convertir la matriz a una lista de listas de enteros
# valores_matriz = [list(map(lambda x: int(x, 0) if x.strip() else 0, line.strip().split(','))) for line in lineas_matriz]
#
# # Crear la matriz de NumPy desde la lista de listas de enteros
# test_img_128x128 = np.array(valores_matriz, dtype=np.uint16)
#
# # Invertir las filas verticalmente
# test_img_128x128 = np.flipud(test_img_128x128)
#
# # Cambiar el orden de los canales de color de RGB a BGR usando operaciones bit a bit
# test_img_bgr565 = ((test_img_128x128 & 0x1F) << 11) | ((test_img_128x128 & 0x7E0)) | ((test_img_128x128 & 0xF800) >> 11)
#
#
#
# # Tamaño de la matriz
# filas, columnas = test_img_128x128.shape
#
# # ... (Código anterior)
#
# # # Convertir la matriz a una lista de listas de enteros en formato BGR655
# # test_img_bgr655 = ((test_img_128x128 & 0x1F) << 11) | ((test_img_128x128 & 0x3E0) << 1) | ((test_img_128x128 & 0xF800) >> 10)
# #
# # # Convertir la matriz a una lista de listas de enteros en formato BGR556
# # test_img_bgr556 = ((test_img_128x128 & 0x1F) << 10) | ((test_img_128x128 & 0x7E0) << 1) | ((test_img_128x128 & 0xF800) >> 11)
#
# # Convertir las nuevas matrices a NumPy
# test_img_bgr565 = np.array(test_img_bgr565, dtype=np.uint16)
# # test_img_bgr655 = np.array(test_img_bgr655, dtype=np.uint16)
# # test_img_bgr556 = np.array(test_img_bgr556, dtype=np.uint16)
#
# # Inicializar Pygame para las nuevas ventanas
# pygame.init()
#
# # Configurar las pantallas para BGR565, BGR655 y BGR556
# pantalla_bgr565 = pygame.display.set_mode((columnas, filas))
# pygame.display.set_caption("Imagen BGR565")
#
# # pantalla_bgr655 = pygame.display.set_mode((columnas, filas))
# # pygame.display.set_caption("Imagen BGR655")
# #
# # pantalla_bgr556 = pygame.display.set_mode((columnas, filas))
# # pygame.display.set_caption("Imagen BGR556")
#
# # Convertir las matrices a superficies de Pygame
# imagen_surface_bgr565 = pygame.surfarray.make_surface(test_img_bgr565)
# # imagen_surface_bgr655 = pygame.surfarray.make_surface(test_img_bgr655)
# # imagen_surface_bgr556 = pygame.surfarray.make_surface(test_img_bgr556)
#
# # Bucle principal para cada ventana
# ejecutando_bgr565 = True
# while ejecutando_bgr565:
#     for evento in pygame.event.get():
#         if evento.type == pygame.QUIT:
#             ejecutando_bgr565 = False
#
#     # Dibujar la superficie en la pantalla
#     pantalla_bgr565.blit(imagen_surface_bgr565, (0, 0))
#     pygame.display.flip()
#
# # Bucle principal para la ventana BGR655
# # ejecutando_bgr655 = True
# # while ejecutando_bgr655:
# #     for evento in pygame.event.get():
# #         if evento.type == pygame.QUIT:
# #             ejecutando_bgr655 = False
# #
# #     # Dibujar la superficie en la pantalla
# #     pantalla_bgr655.blit(imagen_surface_bgr655, (0, 0))
# #     pygame.display.flip()
# #
# # # Bucle principal para la ventana BGR556
# # ejecutando_bgr556 = True
# # while ejecutando_bgr556:
# #     for evento in pygame.event.get():
# #         if evento.type == pygame.QUIT:
# #             ejecutando_bgr556 = False
# #
# #     # Dibujar la superficie en la pantalla
# #     pantalla_bgr556.blit(imagen_surface_bgr556, (0, 0))
# #     pygame.display.flip()
#
# # Salir de Pygame al finalizar
# pygame.quit()
########################################################################################################################
#INFORMACION DE IMAGEN
# import numpy as np
# import matplotlib.pyplot as plt
#
# # Supongamos que ya tienes la matriz cargada en test_img_128x128
# # test_img_128x128 = ...
# ruta_archivo = 'matriz_limpia.txt'
# # Leer el archivo y obtener las líneas
# with open(ruta_archivo, 'r') as file:
#     lines = file.readlines()
#
# # Filtrar solo las líneas relevantes para la matriz
# lineas_matriz = [line for line in lines if 'const uint16_t' not in line]
#
# # Convertir las líneas en una lista de listas de enteros
# valores_matriz = [list(map(lambda x: int(x, 0) if x.strip() else 0, line.strip().split(','))) for line in lineas_matriz]
#
# # Crear la matriz de NumPy desde la lista de listas de enteros
# test_img_128x128 = np.array(valores_matriz, dtype=np.uint16)
#
# # Invertir las filas verticalmente
# test_img_128x128 = np.flipud(test_img_128x128)
#
# # Profundidad del píxel
# profundidad_pixel = test_img_128x128.dtype.itemsize * 8  # En bits
#
# # Tamaño de la matriz
# tamaño_matriz = test_img_128x128.shape
#
# # Color promedio
# color_promedio = np.mean(test_img_128x128, axis=(0, 1))
#
# # Histograma de colores
# histograma = np.histogram(test_img_128x128.flatten(), bins=range(0, 65536))
#
# # Valor máximo y mínimo de píxel
# valor_maximo = np.max(test_img_128x128)
# valor_minimo = np.min(test_img_128x128)
#
# # Desviación estándar
# desviacion_estandar = np.std(test_img_128x128)
#
# # Visualización de la imagen
# plt.imshow(test_img_128x128, cmap='plasma')
# plt.title("Imagen Original")
# plt.show()
#
# # Imprimir resultados
# print(f"Profundidad del Píxel: {profundidad_pixel} bits")
# print(f"Tamaño de la Matriz: {tamaño_matriz}")
# print(f"Color Promedio: {color_promedio}")
# print("Histograma de Colores:")
# print(histograma)
# print(f"Valor Máximo de Píxel: {valor_maximo}")
# print(f"Valor Mínimo de Píxel: {valor_minimo}")
# print(f"Desviación Estándar: {desviacion_estandar}")

# import pygame
# import numpy as np
#
# # Ruta al archivo que contiene la matriz
# ruta_archivo = 'matriz_limpia.txt'
#
# # Leer la matriz desde el archivo
# with open(ruta_archivo, 'r') as file:
#     lines = file.readlines()
#
# # Filtrar solo las líneas que contienen valores de la matriz
# lineas_matriz = [line for line in lines if 'const uint16_t' not in line]
#
# # Convertir la matriz a una lista de listas de enteros
# valores_matriz = [list(map(lambda x: int(x, 0) if x.strip() else 0, line.strip().split(','))) for line in lineas_matriz]
#
# # Crear la matriz de NumPy desde la lista de listas de enteros
# test_img_128x128 = np.array(valores_matriz, dtype=np.uint16)
#
# # Invertir las filas verticalmente
# test_img_128x128 = np.flipud(test_img_128x128)
#
# # Cambiar el orden de los canales de color de RGB a BGR usando operaciones bit a bit
# test_img_bgr565 = ((test_img_128x128 & 0x1F) << 11) | ((test_img_128x128 & 0x7E0)) | ((test_img_128x128 & 0xF800) >> 11)
#
# # Tamaño de la matriz
# filas, columnas = test_img_128x128.shape
#
# # Convertir las nuevas matrices a NumPy
# test_img_bgr565 = np.array(test_img_bgr565, dtype=np.uint16)
#
# # Inicializar Pygame para las nuevas ventanas
# pygame.init()
#
# # Configurar las pantallas para BGR565, BGR655 y BGR556
# pantalla_bgr565 = pygame.display.set_mode((columnas, filas))
# pygame.display.set_caption("Imagen BGR565")
#
#
# # Convertir las matrices a superficies de Pygame
# imagen_surface_bgr565 = pygame.surfarray.make_surface(test_img_bgr565)
#
# # Bucle principal para cada ventana
# ejecutando_bgr565 = True
# while ejecutando_bgr565:
#     for evento in pygame.event.get():
#         if evento.type == pygame.QUIT:
#             ejecutando_bgr565 = False
#
#     # Dibujar la superficie en la pantalla
#     pantalla_bgr565.blit(imagen_surface_bgr565, (0, 0))
#     pygame.display.flip()
#
# pygame.quit()

# def analizar_matriz(matriz_hex):
#     # Dividir la matriz en filas
#     filas = [fila.split(',') for fila in matriz_hex.strip().split('\n')]
#
#     # Extraer algunos píxeles para análisis
#     píxeles_muestra = [fila[0] for fila in filas[:3]]
#
#     # Intentar diferentes órdenes de canales y profundidades de bits
#     posibles_formatos = [
#         ('RGB', 5, 6, 5),
#         ('RGB', 5, 5, 6),
#         ('RGB', 6, 5, 5),
#         ('BGR', 5, 6, 5),
#         ('BGR', 5, 5, 6),
#         ('BGR', 6, 5, 5),
#         ('GBR', 5, 6, 5),
#         ('GBR', 5, 5, 6),
#         ('GBR', 6, 5, 5),
#         # Agrega más formatos según sea necesario
#     ]
#
#     for formato, bits_rojo, bits_verde, bits_azul in posibles_formatos:
#         píxeles_convertidos = []
#
#         for pixel_hex in píxeles_muestra:
#             valor_decimal = int(pixel_hex, 16)
#             rojo = (valor_decimal >> (bits_verde + bits_azul)) & ((1 << bits_rojo) - 1)
#             verde = (valor_decimal >> bits_azul) & ((1 << bits_verde) - 1)
#             azul = valor_decimal & ((1 << bits_azul) - 1)
#             píxeles_convertidos.append((rojo, verde, azul))
#
#         print(f'Formato: {formato}, Píxeles Convertidos: {píxeles_convertidos}')
#
#
# # Ejemplo de uso con una matriz de datos hexadecimales
# def analizar_matriz_desde_archivo(ruta_archivo):
#     # Leer la matriz desde el archivo
#     with open(ruta_archivo, 'r') as archivo:
#         contenido_matriz = archivo.read()
#
#     # Llamar a la función de análisis
#     analizar_matriz(contenido_matriz)
#
#
# # Ejemplo de uso con un archivo de texto
# ruta_archivo_ejemplo = 'matriz_limpia.txt'
# analizar_matriz_desde_archivo(ruta_archivo_ejemplo)
# import pygame
# import numpy as np
#
# # Función para convertir y mostrar una imagen
# def mostrar_imagen(matriz, formato, titulo):
#     # Tamaño de la matriz
#     filas, columnas = matriz.shape
#
#     # Convertir la matriz a formato deseado
#     if formato == 'RGB':
#         # imagen = ((matriz & 0x1F) << 11) | ((matriz & 0x7E0)) | ((matriz & 0xF800) >> 11)
#         # imagen = ((matriz & 0x1F) << 11) | ((matriz & 0x3E0) << 5) | ((matriz & 0x7C00) >> 10)
#         imagen = ((matriz & 0xFC00) >> 10) | ((matriz & 0x7E0) << 1) | ((matriz & 0x1F) << 11)
#
#     elif formato == 'BGR':
#         ##565
#         # imagen = ((matriz & 0x1F) << 11) | ((matriz & 0x3F80) << 5) | ((matriz & 0xF800) >> 11)
#         imagen = ((matriz & 0x1F) << 11) | ((matriz & 0x7E0)) | ((matriz & 0xF800) >> 11)
#         #655
#         # imagen = ((matriz & 0xFC00) >> 10) | ((matriz & 0x7C0) << 1) | ((matriz & 0x1F) << 11)
#         # imagen = ((matriz & 0xFC00) >> 10) | ((matriz & 0x7E0) << 1) | ((matriz & 0x1F) << 11)
#         #556
#         # imagen = ((matriz & 0x1F) << 11) | ((matriz & 0x3E0) << 5) | ((matriz & 0x7C00) >> 10)
#
#     elif formato == 'GBR':
#         # imagen = ((matriz & 0x3E0) << 5) | ((matriz & 0xF800) >> 11) | ((matriz & 0x1F) << 11)
#         imagen = ((matriz & 0x7E0) << 1) | ((matriz & 0xF800) >> 11) | ((matriz & 0x1F) << 11)
#
#         # imagen = ((matriz & 0x7E0)) | ((matriz & 0xF800) >> 11) | ((matriz & 0x1F) << 11)
#     else:
#         raise ValueError(f'Formato no válido: {formato}')
#
#     # Inicializar Pygame
#     pygame.init()
#
#     # Configurar la pantalla
#     pantalla = pygame.display.set_mode((columnas, filas))
#     pygame.display.set_caption(f"Imagen {formato}")
#
#     # Convertir la matriz a superficie de Pygame
#     imagen_surface = pygame.surfarray.make_surface(np.array(imagen, dtype=np.uint16))
#
#     # Bucle principal para la ventana
#     ejecutando = True
#     while ejecutando:
#         for evento in pygame.event.get():
#             if evento.type == pygame.QUIT:
#                 ejecutando = False
#
#         # Dibujar la superficie en la pantalla
#         pantalla.blit(imagen_surface, (0, 0))
#         pygame.display.flip()
#
#     pygame.quit()
#
# # Ruta al archivo que contiene la matriz
# ruta_archivo = 'matriz_limpia.txt'
#
# # Leer la matriz desde el archivo
# with open(ruta_archivo, 'r') as file:
#     lines = file.readlines()
#
# # Filtrar solo las líneas que contienen valores de la matriz
# lineas_matriz = [line for line in lines if 'const uint16_t' not in line]
#
# # Convertir la matriz a una lista de listas de enteros
# valores_matriz = [list(map(lambda x: int(x, 0) if x.strip() else 0, line.strip().split(','))) for line in lineas_matriz]
#
# # Crear la matriz de NumPy desde la lista de listas de enteros
# test_img_128x128 = np.array(valores_matriz, dtype=np.uint16)
#
# # Invertir las filas verticalmente
# test_img_128x128 = np.flipud(test_img_128x128)
#
# # Mostrar imágenes con diferentes formatos
# mostrar_imagen(test_img_128x128, 'RGB', 'RGB')
# mostrar_imagen(test_img_128x128, 'BGR', 'BGR')
# mostrar_imagen(test_img_128x128, 'GBR', 'GBR')
#
# from PIL import Image
# import sys
#
# fname = 'P.png'
#
# img = Image.open(fname)
# if img.width != 128 or img.height != 128:
#     print("Error: 128x128 image expected");
#     sys.exit(2)
#
# print("const uint16_t test_img_128x128[][128] = {");
#
# for y in range(0, img.height):
#     s = "{"
#     for x in range(0, img.width):
#         (r, g, b) = img.getpixel( (x, y) )
#         color565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3)
#         # for right endiness, so ST7735_DrawImage would work
#         color565 = ((color565 & 0xFF00) >> 8) | ((color565 & 0xFF) << 8)
#         s += "0x{:04X},".format(color565)
#     s += "},"
#     print(s)
#
# print("};")

# original_image_path = 'P.bmp'

from PIL import Image
import sys

fname = 'espacio.png'
img = Image.open(fname)

if img.width != 128 or img.height != 160:
    print("Error: 128x160 image expected")
    sys.exit(2)

# Convertir la imagen a modo RGB si no está en ese modo
if img.mode != 'RGB':
    img = img.convert('RGB')

print("const uint16_t marlene[][128] = {");

for y in range(0, img.height):
    s = "{"
    for x in range(0, img.width):
        (r, g, b) = img.getpixel((x, y))
        color565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3)
        color565 = ((color565 & 0xFF00) >> 8) | ((color565 & 0xFF) << 8)
        s += "0x{:04X},".format(color565)
    s += "},"
    print(s)

print("};")
