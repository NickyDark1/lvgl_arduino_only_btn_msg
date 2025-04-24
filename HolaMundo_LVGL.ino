/*
 * HolaMundo_LVGL.ino
 * 
 * Ejemplo básico de LVGL: Botón con mensaje "Hola Mundo"
 */

#include <Arduino.h>
#include <Wire.h>
#include "LVGL_Driver.h"
#include "Display_SPD2010.h"
#include "Touch_SPD2010.h"
#include "TCA9554PWR.h"
#include "I2C_Driver.h"

// Declaración para definir ESP_OK si no está definido
#ifndef ESP_OK
#define ESP_OK 0
#endif

// Declaración para simular la función del ESP-IDF si no está disponible
#ifndef esp_rom_delay_us
#define esp_rom_delay_us(us) delayMicroseconds(us)
#endif

// Declaración de objetos LVGL
lv_obj_t *btn;          // Botón principal
lv_obj_t *label;        // Etiqueta del botón
lv_obj_t *msgbox = NULL; // Mensaje emergente

// Callback para el evento del botón "Cerrar" en el mensaje
static void msgbox_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *mbox = lv_event_get_current_target(e);
  
  if (code == LV_EVENT_VALUE_CHANGED) {
    // Obtener el texto del botón presionado
    const char *txt = lv_msgbox_get_active_btn_text(mbox);
    if (txt) {
      // Cerrar el mensaje
      lv_msgbox_close(mbox);
      msgbox = NULL;
    }
  }
}

// Callback para el evento del botón principal
static void btn_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  
  if (code == LV_EVENT_CLICKED) {
    // Si ya existe un msgbox, no crear otro
    if (msgbox != NULL) {
      return;
    }
    
    // Botones para el mensaje
    static const char *btns[] = {"Cerrar", ""};
    
    // Crear el mensaje
    msgbox = lv_msgbox_create(NULL, "Mensaje", "¡Hola Mundo!", btns, false);
    
    // Centrar el mensaje en la pantalla
    lv_obj_align(msgbox, LV_ALIGN_CENTER, 0, 0);
    
    // Asignar el callback al mensaje
    lv_obj_add_event_cb(msgbox, msgbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    
    // Fondo semi-transparente
    lv_obj_t *bg = lv_obj_get_parent(msgbox);
    lv_obj_set_style_bg_opa(bg, LV_OPA_70, 0);
    lv_obj_set_style_bg_color(bg, lv_palette_main(LV_PALETTE_GREY), 0);
  }
}

// Función para el loop de drivers (usado en la tarea secundaria)
void Driver_Loop(void *parameter) {
  while(1) {
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void setup()
{
  // Inicializar serial para depuración
  Serial.begin(115200);
  Serial.println("Iniciando ejemplo LVGL Hola Mundo");
  
  // Inicializar controladores
  I2C_Init();
  TCA9554PWR_Init(0x00);
  Backlight_Init();
  Set_Backlight(50);

  // Inicializar pantalla
  LCD_Init();
  
  // Inicializar LVGL
  Lvgl_Init();
  
  // Crear un botón
  btn = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn, 150, 50);
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);
  
  // Crear etiqueta para el botón
  label = lv_label_create(btn);
  lv_label_set_text(label, "Haz clic aquí");
  lv_obj_center(label);
  
  // Crear tarea en segundo núcleo para otros controladores
  xTaskCreatePinnedToCore(
    Driver_Loop,     
    "Other Driver task",   
    2048,                
    NULL,                 
    3,                    
    NULL,                
    0                    
  );
  
  Serial.println("Inicialización completada");
}

void loop()
{
  // Actualizar LVGL
  Lvgl_Loop();
  
  // Pequeño retardo
  vTaskDelay(pdMS_TO_TICKS(5));
}