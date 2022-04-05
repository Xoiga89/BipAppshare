/*
   Application template for Amazfit Bip BipOS
   (C) Maxim Volkov  2019 <Maxim.N.Volkov@ya.ru>

   Game 2048 for Amazfit Bip BipOS
   (C) Claudio Benvenuti  2019 <claudio.benvenuti@gmail.com>

*/
// Này là bản cho đồng hồ ở Viet Nam
#include "libbip.h"
#include "2048.h"
#include "BipStatusbarLib.h"

struct regmenu_ screen_data = {
  55,
  1,
  0,
  dispatch_screen,
  key_press_screen,
  screen_job,
  0,
  show_screen,
  0,
  0
};

int main(int param0, char** argv){
  show_screen((void*) param0);
}

void show_screen (void *param0) {
  struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	pointer to a pointer to screen data
  struct app_data_ *	app_data;					//	pointer to screen data
  if ( (param0 == *app_data_p) && get_var_menu_overlay()){
    app_data = *app_data_p;
    *app_data_p = NULL;
    reg_menu(&screen_data, 0);
    *app_data_p = app_data;
  } else {
    reg_menu(&screen_data, 0);
    *app_data_p = (struct app_data_ *)pvPortMalloc(sizeof(struct app_data_));
    app_data = *app_data_p;
    _memclr(app_data, sizeof(struct app_data_));
    app_data->proc = param0;
    if ( param0 && app_data->proc->ret_f )
      app_data->ret_f = app_data->proc->elf_finish;
    else
      app_data->ret_f = show_watchface;
	// ở đây chúng tôi thực hiện các hành động cần thiết nếu hàm được khởi chạy lần đầu tiên từ menu: điền vào tất cả các cấu trúc dữ liệu, v.v.
	struct settings_ settings;

	// đọc cài đặt từ bộ nhớ không bay hơi
	ElfReadSettings(app_data->proc->index_listed, &settings, 0, sizeof(settings));
	/*if (settings.pix_per_rec == 0){	// nếu số lượng pixel trên mỗi mục nhập = 0, thì đây là lần chạy đầu tiên
		// đặt giá trị mặc định
		settings.pix_per_rec = 10;
		settings.seconds_between_rec = 30;
		settings.minutes_for_rec = 0;
		settings.backlight = 0;
		settings.nhe = find_min();
		settings.deep = find_max();
		settings.sleep = find_sum();
		settings.tinh = find_avg();
		// ghi các giá trị này vào bộ nhớ không bay hơi
		ElfWriteSettings(app_data->proc->index_listed, &settings, 0, sizeof(settings));
	}*/
	// điền vào cấu trúc dữ liệu ứng dụng
	app_data->status = 0;
	app_data->pix_per_rec = settings.pix_per_rec;
	 j = settings.deep;
		//_sprintf(max, "%d", settings.deep);
	h = settings.nhe;
		//_sprintf(min, "%d", settings.nhe);	
     l = settings.sleep;
		//_sprintf(sum, "%d", settings.sleep);
	k = settings.tinh;	 
		//_sprintf(avg, "%d", settings.tinh);
		afhour = settings.beginhour;
		afmin = settings.beginmin;
		edhour = settings.endhour;
		edmin = settings.endmin;
	app_data->curX = 0;			
	app_data->curY = 176;
	app_data->rec_counter = 0;
	app_data->rec_counter_per_screen = 0;
	if ( settings.minutes_for_rec == 0) {
		app_data->minutes_for_rec = 60;
	}
	else {
		app_data->minutes_for_rec = settings.minutes_for_rec;
	}
	app_data->anim_counter = 0;
	if ( settings.seconds_between_rec == 0) {
		app_data->seconds_between_rec = 60;
	}
	else {
		app_data->seconds_between_rec = settings.seconds_between_rec;
	}
	app_data->menu_stage = 0;
	app_data->curr_time = 0;
	app_data->backlight = settings.backlight;
	app_data->weather_dem = 0;
		app_data->weather_dem_man = 0;
	
	// bật khả năng hiển thị của nút điểm tham chiếu đã đặt
	app_data->store_point_btn_visible = true;
	
	// đặt lại yêu cầu thiết lập điểm tham chiếu đang chờ xử lý
	app_data->store_point_delayed = false;
	
	    app_data->screen = 1;
 app_data->col=0;
 app_data->current_screen = 0;
 app_data->state_hash = -1;

	set_display_state_value(4, 1);	// cài đặt đèn nền: 0 buộc tắt, 1 buộc bật

	// ở đây chúng ta vẽ giao diện, không cần cập nhật (chuyển vào bộ nhớ video) màn hình

}
draw_screen();

// đặt bộ hẹn giờ để gọi screen_job tính bằng mili giây, tùy thuộc vào việc có cần trình bảo vệ màn hình hay không
set_update_period(1,  1000);


// prevent exiting the app after a period
  set_display_state_value(8, 1);
  set_display_state_value(2, 1);

}

//Da kiem tra oke
void key_press_screen(){
  struct app_data_** 	app_data_p = get_ptr_temp_buf_2();
  struct app_data_ *	app_data = *app_data_p;

if ( app_data->screen == 11 ) {
 if (app_data->menu_is_on == 0){
		//	если сейчас проводится измерение, то выводим сводку по измерениям
		set_hrm_mode(0);	// tắt máy đo nhịp tim
		set_update_period(0, 0);	// выключаем таймер обновления экрана
		app_data->menu_stage = 2;
		show_menu_animate(menu, 0, ANIMATE_LEFT);	// отрисовываем меню
	} 
}
else {
	  show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
}
};

void key_long_press(){
struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

if (( app_data->screen == 17 ) || ( app_data->screen == 19 )) {
if (!shut_down_navi()){	// hoạt động cuối cùng (tắt cảm biến, lưu)
	
// nếu khởi chạy từ menu nhanh, thì khi nhấn nút, chúng ta thoát sang quay số
	if ( get_left_side_menu_active() ) 		
		app_data->proc->ret_f = show_watchface;
	
	
	// gọi hàm trả về (thường đây là menu khởi chạy), dưới dạng một tham số, chúng tôi chỉ định địa chỉ của hàm trong ứng dụng của chúng tôi
	show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);	
}
}
};


//Chua copy


int dispatch_screen (void *param){
  struct app_data_** 	app_data_p = get_ptr_temp_buf_2();
  struct app_data_ *	app_data = *app_data_p;
  app_data->last_tick = get_tick_count();
  struct gesture_ *gest = param;
  int result = 0;
 
  switch (gest->gesture){
    case GESTURE_CLICK: {
                          vibrate(1,50,0);
						  
						  if (app_data->menu_is_on == 1){
				//	увеличение или уменьшение значений на экранах настройки
				if (app_data->menu_stage == 1){ //Quãng thời gian đo nhịp tim (giây) cách nhau giũa 2 lần
					if ((gest->touch_pos_x >= 20 && gest->touch_pos_x <= 70) && (gest->touch_pos_y >= 55 && gest->touch_pos_y <= 105)){
						app_data->seconds_between_rec = app_data->seconds_between_rec - 30;
						if (app_data->seconds_between_rec < 0){
							app_data->seconds_between_rec = 60;
						}
						set_fg_color(COLOR_WHITE);
						char text[10];
						_sprintf(text, "%d", app_data->seconds_between_rec);
						draw_filled_rect_bg(FIRST_MENU_BIG_DIGITS_COORD_X, FIRST_MENU_BIG_DIGITS_COORD_Y, 109, FIRST_MENU_BIG_DIGITS_COORD_Y + 50);
						text_out_font(3, text, FIRST_MENU_BIG_DIGITS_COORD_X, FIRST_MENU_BIG_DIGITS_COORD_Y, 2);
						update_settings();
						repaint_screen_lines(FIRST_MENU_BIG_DIGITS_COORD_Y, FIRST_MENU_BIG_DIGITS_COORD_Y + 50);
					}
					if ((gest->touch_pos_x >= 100 && gest->touch_pos_x <= 150) && (gest->touch_pos_y >= 55 && gest->touch_pos_y <= 105)){
						app_data->seconds_between_rec = app_data->seconds_between_rec + 30;
						if (app_data->seconds_between_rec > 3600){
							app_data->seconds_between_rec = 60;
						}
						set_fg_color(COLOR_WHITE);
						char text[10];
						_sprintf(text, "%d", app_data->seconds_between_rec);
						draw_filled_rect_bg(FIRST_MENU_BIG_DIGITS_COORD_X, FIRST_MENU_BIG_DIGITS_COORD_Y, 109, FIRST_MENU_BIG_DIGITS_COORD_Y + 50);
						text_out_font(3, text, FIRST_MENU_BIG_DIGITS_COORD_X, FIRST_MENU_BIG_DIGITS_COORD_Y, 2);
						update_settings();
						repaint_screen_lines(FIRST_MENU_BIG_DIGITS_COORD_Y, FIRST_MENU_BIG_DIGITS_COORD_Y + 50);
					}
				} 
				// xử lý nhấp vào cuối màn hình trong màn hình thiết lập
				if (gest->touch_pos_y >= 135 && gest->touch_pos_y <= 176){
					if (app_data->menu_stage == 0){ //Bật đèn nền hay không
						if (gest->touch_pos_x >= 0 && gest->touch_pos_x < 88){
							app_data->backlight = 0;
							set_backlight_state(0);
							update_settings();
							app_data->menu_stage = app_data->menu_stage + 1;
							show_menu_animate(menu, 0, ANIMATE_LEFT);
						} else if (gest->touch_pos_x > 88 && gest->touch_pos_x <= 176){
							app_data->backlight = 1;
							set_backlight_state(1);
							update_settings();
							app_data->menu_stage = app_data->menu_stage + 1;
							show_menu_animate(menu, 0, ANIMATE_LEFT);
						}
					} else if (app_data->menu_stage == 1){ //Đến màn setting cuối cùng rồi thì tắt setting nên menu_setting = 0, ở đây bin chọn 2 màn setting là bật nền và khoảng thời gian đo thôi nên có 2 màn hình thôi
						app_data->menu_is_on = 0;
						app_data->menu_stage = 0;
						update_settings();	//	обновление настроек в энергонезависимой памяти
						show_menu_animate(redraw_screen, 0, ANIMATE_UP);	// sau khi thiết lập, bắt đầu vẽ lại biểu đồ
					} else if (app_data->menu_stage == 2){//Trên menu tổng kết kết quả
						// nếu khởi chạy từ menu nhanh, thì khi nhấn nút, chúng ta thoát sang quay số
						if ( get_left_side_menu_active() ) 		
						    app_data->proc->ret_f = show_watchface;
									struct datetime_ dt;
									get_current_date_time(&dt);
									 endenmin = dt.min;
									 endenhour = dt.hour;
									update_settings();
						// gọi hàm trả về (thường đây là menu khởi chạy), dưới dạng một tham số, chúng tôi chỉ định địa chỉ của hàm trong ứng dụng của chúng tôi
						show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
					} else {
						app_data->menu_stage = app_data->menu_stage + 1;	// переключаем экран меню
						update_settings();	//	обновление настроек в энергонезависимой памяти
						show_menu_animate(menu, 0, ANIMATE_LEFT);	// заново отрисовываем меню
					}
				}
			}
						  
                          switch (app_data->screen){
                            case 1: {
								
                                      app_data->screen = 2;
									  draw_score_screen();
									 
                                      
                                      break;
                                    }
									case 8: {if ( app_data->screen == 8 ){ 
				 app_data->screen = 9;
				set_bg_color(COLOR_BLACK);
                fill_screen_bg();
                thegioi2();
                                        repaint_screen_lines(0, 176);
                                      break;
			                          }}
									  case 9: {if ( app_data->screen == 9 ){ 
				 app_data->screen = 8;
				set_bg_color(COLOR_BLACK);
                fill_screen_bg();
                thegioi1();
                                        repaint_screen_lines(0, 176);
                                      break;
			                          }}
									  case 10: {if ( app_data->screen == 10 ){ 
				if ( ( gest->touch_pos_y > 100) && ( gest->touch_pos_y <= 140)  && ( gest->touch_pos_x >= 70) &&  ( gest->touch_pos_x <= 110) ){
                                        
										app_data->screen = 11;
                                        set_bg_color(COLOR_BLACK);
                                        fill_screen_bg();
									redraw_screen();
						struct datetime_ dt;
						get_current_date_time(&dt);
						 anfangmin = dt.min;
						 anfanghour = dt.hour;
						update_settings();
									  }
									   repaint_screen_lines(0, 176);
                                      break;
			                          }
									/* else { 
									   set_bg_color(COLOR_BLACK);
                                        fill_screen_bg();
									app_data->screen = 16;
									ketqua();
									 repaint_screen_lines(0, 176);
                                      }*/
									  repaint_screen_lines(0, 176);
									     break;
										}
									  case 16: {if ( app_data->screen == 16 ){ 
									   set_bg_color(COLOR_BLACK);
                                        fill_screen_bg();
									app_data->screen = 10;
									takenap();
									 repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
									  
									    case 11: {if ( app_data->screen == 11 ){ 
				if ( ( gest->touch_pos_y > 100) && ( gest->touch_pos_y <= 140)  && ( gest->touch_pos_x >= 70) &&  ( gest->touch_pos_x <= 110) ){
                                        app_data->screen = 12;
										if (app_data->menu_is_on == 0){
		//	если сейчас проводится измерение, то выводим сводку по измерениям
		set_hrm_mode(0);	// tắt máy đo nhịp tim
		set_update_period(0, 0);	// выключаем таймер обновления экрана
		app_data->menu_stage = 2;
		show_menu_animate(menu, 0, ANIMATE_LEFT);	// отрисовываем меню
			struct datetime_ dt;
									get_current_date_time(&dt);
									 endenmin = dt.min;
									 endenhour = dt.hour;
									update_settings();
										}
									 repaint_screen_lines(0, 176);
                                      }
									   repaint_screen_lines(0, 176);
                                      break;
			                          }}
									  case 12: {if ( app_data->screen == 12 ){ 
									app_data->screen = 1;
                                        set_bg_color(COLOR_BLACK);
                                        fill_screen_bg();
									draw_board();
									 repaint_screen_lines(0, 176);
                                      }
									   repaint_screen_lines(0, 176);
                                      break;
			                          }
									  case 13: {if ( app_data->screen == 13 ){ 
									app_data->screen = 14;
                                        set_bg_color(COLOR_BLACK);
                                        fill_screen_bg();
									tim_cal();
									 repaint_screen_lines(0, 176);
                                      }
									   repaint_screen_lines(0, 176);
                                      break;
			                          }
									   case 14: {if ( app_data->screen == 14 ){ 
									app_data->screen = 15;
                                        set_bg_color(COLOR_BLACK);
                                        fill_screen_bg();
									tim_result();
									 repaint_screen_lines(0, 176);
                                      }
									   repaint_screen_lines(0, 176);
                                      break;
			                          }
									   case 15: {if ( app_data->screen == 15 ){ 
				if ( ( gest->touch_pos_y > 100) && ( gest->touch_pos_y <= 140)  && ( gest->touch_pos_x >= 70) &&  ( gest->touch_pos_x <= 110) ){
                                        
										app_data->screen = 14;
                                        set_bg_color(COLOR_BLACK);
                                        fill_screen_bg();
									tim_cal();
									  }
									  else {
										  app_data->screen = 1;
										  set_hrm_mode(0); 
                                        set_bg_color(COLOR_BLACK);
                                        fill_screen_bg();
									draw_board();
									 repaint_screen_lines(0, 176);
									  }
									   repaint_screen_lines(0, 176);
                                      break;
			                          }}
									  case 17: {if ( app_data->screen == 17 ){ 
										app_data->screen = 18;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										weather_screen();
										switch_gps_pressure_sensors(SENSOR_ENABLE);
											app_data->sensor_mode = SENSOR_ENABLE;
										// lấy dữ liệu điều hướng
												get_navi_data(&app_data->navi_data);

												if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
													
													if (app_data->store_point_delayed){
														
														app_data->store_point_delayed = false;
														app_data->store_point_btn_visible = true;
														
														
													}
												}
                                        repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
									  case 18: {if ( app_data->screen == 18 ){ 
									if ( app_data->weather_dem > 2 ){
									app_data->screen = 19;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										shut_down_navi();
										weather_result();
                                        repaint_screen_lines(0, 176);
                                      }/* else { 
									  										app_data->screen = 18;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										weather_screen();
										switch_gps_pressure_sensors(SENSOR_ENABLE);
											app_data->sensor_mode = SENSOR_ENABLE;
										// lấy dữ liệu điều hướng
												get_navi_data(&app_data->navi_data);

												if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
													
													if (app_data->store_point_delayed){
														
														app_data->store_point_delayed = false;
														app_data->store_point_btn_visible = true;
														
														
													}
												}
									  } */
									  }
									     break;
			                          }
									  /*case 20: {if ( app_data->screen == 17 ){ 
app_data->screen = 20;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										weather1();
                                        repaint_screen_lines(0, 176);
                                      }
									  									     break;
			                          }
									  case 21: {if ( app_data->screen == 20 ){ 
app_data->screen = 21;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										weather2();
                                        repaint_screen_lines(0, 176);
                                      }
									  									     break;
			                          }
									 case 22: {if ( app_data->screen == 21 ){ 
app_data->screen = 19;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										weather_result();
                                        repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }*/
									   case 19: {if ( app_data->screen == 19 ){ 
									app_data->screen = 19;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										shut_down_navi();
										weather_result();
                                        repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
									     case 3: {if ( app_data->screen == 3 ){ 
									app_data->screen = 2;
									  draw_score_screen();
									   }
									   repaint_screen_lines(0, 176);
                                      break;
			                          }
									  case 4: {if ( app_data->screen == 4 ){ 
									app_data->screen = 2;
									  draw_score_screen();
									   }
									   repaint_screen_lines(0, 176);
                                      break;
			                          }
									  case 5: {if ( app_data->screen == 5 ){ 
									app_data->screen = 2;
									  draw_score_screen();
									   }
									   repaint_screen_lines(0, 176);
                                      break;
			                          }
									  case 6: {if ( app_data->screen == 6 ){ 
									app_data->screen = 2;
									  draw_score_screen();
									   }
									   repaint_screen_lines(0, 176);
                                      break;
			                          }
                            case 2: {
                                      if ( ( gest->touch_pos_y >143) && ( gest->touch_pos_y <= 176)  && ( gest->touch_pos_x >= 1) &&  ( gest->touch_pos_x <= 90) ){
                                        
										app_data->screen = 3;
                                        set_bg_color(COLOR_WHITE);
                                        fill_screen_bg();
									
                                        show_elf_res_by_id(app_data->proc->index_listed, 0, 2, 2);   //SSKDT
                                        set_bg_color(COLOR_WHITE);
                                        set_fg_color(COLOR_BLACK);
                                
                                        repaint_screen_lines(0, 176);
                                      } else if ( ( gest->touch_pos_y >143) && ( gest->touch_pos_y <= 176)  && ( gest->touch_pos_x > 90) &&  ( gest->touch_pos_x <= 176) ){
                                        
										app_data->screen = 4;
                                        set_bg_color(COLOR_WHITE);
                                        fill_screen_bg();
                                        show_elf_res_by_id(app_data->proc->index_listed, 1, 2, 2);  //PC COVID
                                        set_bg_color(COLOR_WHITE);
                                        set_fg_color(COLOR_BLACK);
                                
                                        repaint_screen_lines(0, 176);
										} else if ( ( gest->touch_pos_y >50) && ( gest->touch_pos_y < 85)  && ( gest->touch_pos_x >= 1) &&  ( gest->touch_pos_x <= 176) ){
                                        
										app_data->screen = 5;
                                        set_bg_color(COLOR_WHITE);
                                        fill_screen_bg();
                                        show_elf_res_by_id(app_data->proc->index_listed, 2, 0, 0);    //TONY
                                        set_bg_color(COLOR_WHITE);
                                        set_fg_color(COLOR_BLACK);
                                
                                        repaint_screen_lines(0, 176);
                                      } else if ( ( gest->touch_pos_y >90) && ( gest->touch_pos_y < 133)  && ( gest->touch_pos_x >= 1) &&  ( gest->touch_pos_x <= 90) ){
                                        
										app_data->screen = 6;
										set_bg_color(COLOR_BLACK);
                                        fill_screen_bg();
										ask_confirmation(); //Emergency
										set_bg_color(COLOR_BLACK);
                                        set_fg_color(COLOR_WHITE);
										
                                                           }else if ( ( gest->touch_pos_y >90) && ( gest->touch_pos_y < 133)  && ( gest->touch_pos_x > 90) &&  ( gest->touch_pos_x <= 176) ){
                                        
										app_data->screen = 7;
										set_bg_color(COLOR_BLUE);
                                        fill_screen_bg();
										 chon_lua(); //Chonlua
										 set_bg_color(COLOR_BLUE);
                                       
									
                                           
                                      } else {
                                       
									 app_data->screen = 8;
										thegioi1();
								
                                      }
                                      break;
                                    }
                            case 30: {
                                      
									  app_data->screen = 1;
									  draw_board();
								
                                      break;
                                    }
case 7: {if ( app_data->screen == 7 ){ 
				 app_data->screen = 7;
				set_bg_color(COLOR_BLUE);
                                        fill_screen_bg();
										 chon_lua(); //Chonlua
										 set_bg_color(COLOR_BLUE);
                                        repaint_screen_lines(0, 176);
                                      break;
			 }}
										
										app_data->screen = 1;
										draw_board();
							  
                                      break;
                    
	
                          }

                          break;
                        };
    case GESTURE_SWIPE_DOWN: {
                               
                               app_data->screen = 1;
							   draw_board();
							 

                               break;
                             };
    case GESTURE_SWIPE_UP: {
		 switch (app_data->screen){
									  case 16: {if ( app_data->screen == 16 ){ 
										iwdg_reboot();
                                        repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
									  case 3: {if ( app_data->screen == 14 ){ 
									   repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
									  case 2: {if ( app_data->screen == 2 ){ 
app_data->screen = 17;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										weather();
										switch_gps_pressure_sensors(SENSOR_ENABLE);
											app_data->sensor_mode = SENSOR_ENABLE;
										// lấy dữ liệu điều hướng
												get_navi_data(&app_data->navi_data);

												if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
													
													if (app_data->store_point_delayed){
														
														app_data->store_point_delayed = false;
														app_data->store_point_btn_visible = true;
														
														
													}
												}
                                        repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
									  case 17: {if ( app_data->screen == 17 ){ 
app_data->screen = 18;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										weather_screen();
										switch_gps_pressure_sensors(SENSOR_ENABLE);
											app_data->sensor_mode = SENSOR_ENABLE;
										// lấy dữ liệu điều hướng
												get_navi_data(&app_data->navi_data);

												if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
													
													if (app_data->store_point_delayed){
														
														app_data->store_point_delayed = false;
														app_data->store_point_btn_visible = true;
														
														
													}
												}
                                        repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
									  case 18: {if ( app_data->screen == 18 ){ 
app_data->screen = 19;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										shut_down_navi();
										weather_result();
                                        repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
									  /*
									  case 17: {if ( app_data->screen == 17 ){ 
app_data->screen = 20;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										weather1();
                                        repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
									  case 20: {if ( app_data->screen == 20 ){ 
app_data->screen = 21;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										weather2();
                                        repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
									  case 21: {if ( app_data->screen == 21 ){ 
app_data->screen = 19;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										weather_result();
                                        repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }*/
									  case 19: {if ( app_data->screen == 19 ){ 
app_data->screen = 17;									 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										weather();
										switch_gps_pressure_sensors(SENSOR_ENABLE);
											app_data->sensor_mode = SENSOR_ENABLE;
										// lấy dữ liệu điều hướng
												get_navi_data(&app_data->navi_data);

												if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
													
													if (app_data->store_point_delayed){
														
														app_data->store_point_delayed = false;
														app_data->store_point_btn_visible = true;
														
														
													}
												}
                                        repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
									  case 50: {if ( ( app_data->screen != 18 ) && ( app_data->screen != 19) && ( app_data->screen != 17 ) && ( app_data->screen != 2 ) && ( app_data->screen != 14 ) ){ 
									app_data->screen = 8;		 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										thegioi1();	
                                        repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
									   case 51: {if (app_data->screen == 1){ 
									app_data->screen = 8;		 
									 set_bg_color(COLOR_BLACK);
										fill_screen_bg();
										thegioi1();	
                                        repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
		
                             app_data->screen = 8;
							thegioi1();
							 break;
                    
	
                          }
                             break;
                           };
    case GESTURE_SWIPE_LEFT: {
                             
                               show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
                               break;
                             };
    case GESTURE_SWIPE_RIGHT: {
                              switch (app_data->screen){
								  
								   case 1: {if ( app_data->screen == 1 ){ 
									   set_bg_color(COLOR_BLACK);
                                        fill_screen_bg();
									app_data->screen = 16;
									ketqua();
									 repaint_screen_lines(0, 176);
                                      }
									     break;
			                          }
									  case 2: {if ( app_data->screen == 2 ){ 
									   set_bg_color(COLOR_BLACK);
                                        fill_screen_bg();
									app_data->screen = 13;
									tim();
									 repaint_screen_lines(0, 176);
                                      }
									 
                                      break;
			                          }
									  case 19: {if ( app_data->screen == 19 ){ 
									   set_bg_color(COLOR_BLACK);
                                        fill_screen_bg();
									app_data->screen = 17;
									weather();
									switch_gps_pressure_sensors(SENSOR_ENABLE);
											app_data->sensor_mode = SENSOR_ENABLE;
										// lấy dữ liệu điều hướng
												get_navi_data(&app_data->navi_data);

												if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
													
													if (app_data->store_point_delayed){
														
														app_data->store_point_delayed = false;
														app_data->store_point_btn_visible = true;
														
														
													}
												}
									 repaint_screen_lines(0, 176);
                                      }
									 
                                      break;
			                          }
									 
									  case 10: {if ( app_data->screen == 10 ){ 
									   set_hrm_mode(0);	// tắt máy đo nhịp tim
			set_update_period(0, 0);	// выключаем таймер обновления экрана
			show_menu_animate(menu, 0, ANIMATE_DOWN);	// отрисовываем меню
                                      }
									 
                                      break;
			                          }
							
							  }                              
								break;
                              };
    default:{
              vibrate(1,70,0);
              break;
            };

  }
  return result;
};


void draw_board(){
 set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  load_font();
		// отрисовка значений
		set_fg_color(COLOR_WHITE);
		text_out_center("I’m Tony Nguyen \nv.gd/i1DGK5 \nm.me/tony99.inc", 88, 5);
		text_out_center("VCB:004.100.030.2243 \nHo chieu: 11/09/17 \nVNM C382.1283", 88, 110);
		char clock_time[8]; 			//	текст время		12:34
		struct datetime_ dt;
		get_current_date_time(&dt);
		_sprintf(clock_time, "%02d:%02d:%02d", dt.hour, dt.min, dt.sec);
		text_out_font(4, clock_time, 20 , 70, 6); // печатаем результат(время) большими цифрами
		repaint_screen_lines(0, 176); // обновляем строки экрана

};

void tim(){
		struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  struct app_data_ *app_data = *app_data_p;
	set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  set_hrm_mode(0); 	//	tắt màn hình nhịp tim để tiết kiệm pin (không hoạt động với độ trễ từ 0 - 10 giây)
  load_font();
       show_statusbar(3, COLOR_BLACK,COLOR_WHITE);	// статус бар
	
	char crnd[8];     // переменная для перевода переменной col типа int в  тип char для печати                               
    _sprintf(crnd, "%07d", COLOR_WHITE); // конвертируем число int в char лидирующие нули, при необходимости, _sprintf добьёт сам
	set_bg_color(COLOR_BLACK); // делаем фон черным
	draw_filled_rect_bg(0, 43, 176, 110); // стираем предыдущее значение
		// отрисовка значений
		set_fg_color(COLOR_WHITE);
		text_out_center("Tinh V02 max", 88, 35);
		text_out_center("By Tony", 88, 55);
  set_bg_color(COLOR_AQUA);
  set_fg_color(COLOR_BLACK);
  draw_filled_rect_bg(70, 90, 102, 122);
   set_bg_color(COLOR_AQUA);
    set_fg_color(COLOR_BLACK);
  text_out("Do Nhip Tim", 75, 105); 
set_update_period(1, 60000);
    repaint_screen_lines(0, 176); // обновляем строки экрана
};

int tim_cal(){
		struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  struct app_data_ *app_data = *app_data_p;
  set_backlight_state(0);
	set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  load_font();
        set_fg_color(COLOR_WHITE);
  text_out_center("Loading...", 80, 80); 
    set_hrm_mode(0x20);	// bật máy theo dõi nhịp tim
	find_hr();
	find_vmax();
	 set_fg_color(COLOR_GREEN);
    draw_filled_rect_bg(50, 100, 102, 132);
   set_bg_color(COLOR_GREEN);
     set_fg_color(COLOR_BLACK);
  text_out("Ket qua do", 60, 110);  
return 0;
};

int find_hr(){	//	функция поиска минимального значения в массиве
	struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  struct app_data_ *app_data = *app_data_p;
 
    set_hrm_mode(0x20);	// bật máy theo dõi nhịp tim
	void* hrm_data = get_hrm_struct(); 
int heartrate;


if (get_fw_version() == NOT_LATIN_1_1_2_05){
	app_data->status = ((hrm_data_struct_legacy*)hrm_data)->ret_code;
	heartrate = ((hrm_data_struct_legacy*)hrm_data)->heart_rate;

} else {
	app_data->status = ((hrm_data_struct*)hrm_data)->ret_code;
	heartrate = ((hrm_data_struct*)hrm_data)->heart_rate;


}
	return heartrate;	//	возвращаем минимальное значение
}

int find_vmax(){	//	функция поиска минимального значения в массиве
	struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  struct app_data_ *app_data = *app_data_p;
 
    set_hrm_mode(0x20);	// bật máy theo dõi nhịp tim
	void* hrm_data = get_hrm_struct(); 
int heartrate;


if (get_fw_version() == NOT_LATIN_1_1_2_05){
	app_data->status = ((hrm_data_struct_legacy*)hrm_data)->ret_code;
	heartrate = ((hrm_data_struct_legacy*)hrm_data)->heart_rate;

} else {
	app_data->status = ((hrm_data_struct*)hrm_data)->ret_code;
	heartrate = ((hrm_data_struct*)hrm_data)->heart_rate;


}


	int vmax;
	struct datetime_ dt;
		get_current_date_time(&dt);
		
		vmax = (15*(220-(dt.year-1999))/heartrate);   //1999 để tính tuổi  
		// Lưu ý với máy tính của Bin thì viết số nguyên là có dùng dấu phẩy ( , ) không phải dấu chấm ( . )
//vmax = 15,3 *(205,8 -(0,685 *(dt.year-1999)))/heartrate;   //1999 để tính tuổi  
	return vmax;	//	возвращаем минимальное значение
}

void tim_result(){
	set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  set_backlight_state(1);

set_hrm_mode(0); 	//	tắt màn hình nhịp tim để tiết kiệm pin (không hoạt động với độ trễ từ 0 - 10 giây)
		set_fg_color(COLOR_WHITE);
		text_out_center("Ket qua do: ", 88, 35); //V02max
		text_out("V02max: ", 10, 55); //V02max
		text_out("Nhip tim: ", text_width("V02max") +30, 55); //Nhịp tim
			

        //Hàm show_big_digit(Mã font, Đoạn code cần hiện, Trục x, Trục y, khoảng cách)
        //Hàm Text_out_font(Mã font, đoạn mã cần hiển thị, trục x, trục y, khoảng cách)		
		char hr[10];
		_sprintf(hr, "%d", find_hr());
		text_out_font(10, hr, text_width("V02max") +35, 75, 2);

		char vmax[10];
		_sprintf(vmax, "%d", find_vmax());
		text_out_font(10, vmax, 15, 75, 2);
	
	 set_fg_color(COLOR_WHITE);
    draw_filled_rect_bg(70, 100, 102, 132);
   set_bg_color(COLOR_WHITE);
     set_fg_color(COLOR_BLACK);
  text_out("Do lai", 80, 110); 	
		
};

/*
Link tham khảo cho đo V02 max:
https://adventure.howstuffworks.com/outdoor-activities/triathlons/training/vo2-max1.htm
https://www.omnicalculator.com/sports/vo2-max
https://www.wikihow.com/Measure-VO2-Max
VO2 max = 15 x (HRmax/HRrest). This method is considered to compare well with other common formulas.[6]
There is some research that suggests this formula oversimplifies the calculation. You can also estimate your max heart rate with the formula HRmax = 205.8 – (0.685 x age).[5]

*/

/*
Link tham khảo cho đo giấc ngủ theo nhịp tim:
https://vietjack.com/thu-vien-c/ham-sprintf-trong-c.jsp
https://nyxo.app/what-can-heart-rate-tell-about-your-sleep#:~:text=In%20general%2C%20a%20low%20resting,lowest%20point%20of%20the%20curve.
https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3963245/
https://www.healthline.com/health/how-much-deep-sleep-do-you-need#increasing-deep-sleep
https://www.sleepfoundation.org/stages-of-sleep/deep-sleep
https://www.google.com/search?q=Heart+rate+variability+l%C3%A0+g%C3%AC&sxsrf=APq-WBvc2DnKbriAh49quK5Nt8jJePbY_A%3A1647596596080&ei=NFQ0Yta-BKLl2roP7paxmAw&oq=heart+rate+variability+functions&gs_lp=Egdnd3Mtd2l6uAEDKgIIATIHEAAYRxiwAzIHEAAYRxiwAzIHEAAYRxiwAzIHEAAYRxiwAzIHEAAYRxiwAzIHEAAYRxiwAzIHEAAYRxiwAzIHEAAYRxiwA5AGCEjcEFAAWABwAXgByAEAkAEAmAEAoAEAqgEA4gMEIEEYAOIDBCBGGACIBgE&sclient=gws-wiz
https://www.cnet.com/health/sleep/sleeping-heart-rate-breathing-rate-and-hrv-what-your-sleep-data-means/
https://my.clevelandclinic.org/health/symptoms/21773-heart-rate-variability-hrv
https://www.ncbi.nlm.nih.gov/pmc/articles/PMC5624990/#:~:text=Heart%20rate%20variability%20(HRV)%20is,nervous%20system%20(ANS)%20processes.
file:///F:/Download/Heart_Rate_During_Sleep_Implications_for_Monitorin.pdf
https://assets.firstbeat.com/firstbeat/uploads/2019/11/A-Sleep-Analysis-Method-Based-on-Heart-Rate-Variability-071119.pdf
https://www.google.com/search?q=the+change+of+heart+rate+during+sleep&oq=the+change+of+heart+rate+during+sleep&aqs=chrome..69i57j33i22i29i30l3.11427j0j7&sourceid=chrome&ie=UTF-8
https://www.google.com/search?q=heart+rate+change+in+rem+and+non+rem+stage&sxsrf=APq-WBucRWz6Dha9wNiJsj2tT08m4KYsPA%3A1647431776649&ei=YNAxYuKZJ9eF-AbcjJHABQ&ved=0ahUKEwjiydrOycr2AhXXAt4KHVxGBFgQ4dUDCA4&uact=5&oq=heart+rate+change+in+rem+and+non+rem+stage&gs_lp=Egdnd3Mtd2l6uAED-AEBMgUQIRigATIEECEYFcICBBAjGCfCAggQLhixAxiDAcICBBAAGEPCAgQQABgDwgIFEAAYgATCAg4QLhiABBixAxjHARijAsICCBAuGIAEGNQCwgIEEC4YA8ICCBAAGIAEGLEDwgIFEC4YgATCAgsQLhiABBjHARijAsICCBAuGIAEGLEDwgIHEAAYsQMYQ8ICDhAuGIAEGMcBGK8BGNQCwgILEAAYgAQYsQMYgwHCAgsQLhiABBixAxjUAsICBRAAGMsBwgIHEAAYChjLAcICBhAAGBYYHsICCBAAGBYYChgewgIIECEYFhgdGB7CAgcQIRgKGKABSL1mUABY_GRwB3gByAEAkAEAmAGHAqABjjmqAQYwLjQxLjbiAwQgQRgA4gMEIEYYAIgGAQ&sclient=gws-wiz
https://www.ahajournals.org/doi/10.1161/01.cir.91.7.1918
https://www.nature.com/articles/1300146
https://pubmed.ncbi.nlm.nih.gov/17946573/
https://pubmed.ncbi.nlm.nih.gov/17702272/
https://www.health.harvard.edu/blog/how-does-sleep-affect-your-heart-rate-2021012921846
https://ouraring.com/blog/heart-rate-during-sleep/
https://www.google.com/search?q=value+of+heart+rate+that+mean+you+are+in+rem+sleep&oq=value+of+heart+rate+that+mean+you+are+in+rem+sleep&aqs=chrome..69i57j33i160.12222j0j7&sourceid=chrome&ie=UTF-8
https://www.cnet.com/health/sleep/sleeping-heart-rate-breathing-rate-and-hrv-what-your-sleep-data-means/
https://www.sciencedirect.com/science/article/pii/S2405844019361894
https://www.google.com/search?q=difference+between+deep+sleep+and+light+sleep+using+heart+rate&sxsrf=APq-WBvE1wOaZfEELhJcimOLL2TeGscW6Q%3A1647541831893&ei=R34zYuyPNpvChwPY4pv4Bw&oq=difference+between+deep+sleep+and+light+sleep+using+hea&gs_lp=Egdnd3Mtd2l6uAED-AEBKgIIADIFECEYoAEyBRAhGKABMgUQIRigATIFECEYoAHCAgQQIxgnwgIGECMYJxgTwgILEAAYgAQYsQMYgwHCAggQABixAxiDAcICCBAAGIAEGLEDwgIFEAAYgATCAgQQABhDwgIEEC4YQ8ICCxAuGIAEGMcBGK8BwgINEAAYgAQYsQMYRhj_AcICChAAGLEDGIMBGEPCAgUQABjLAcICBxAAGAoYywHCAgkQABgNGEYY_wHCAgQQABgNwgIHEAAYsQMYDcICChAAGLEDGIMBGA3CAgcQABjJAxgNwgIMEAAYsQMYChhGGP8BwgIHEAAYsQMYCsICBBAAGArCAgoQABixAxiDARgKwgIHEC4Y1AIYCsICBBAuGA3CAgYQABgWGB7CAgQQABgTwgIIEAAYFhgeGBPCAgYQABgNGB7CAggQABgIGA0YHsICCBAhGBYYHRgewgIKEAAYCBgNGB4YE8ICBBAhGBXCAgcQIRgKGKABSMSWAVAAWOORAXAPeAHIAQCQAQOYAaQCoAHcOaoBBjEuNDcuMeIDBCBBGADiAwQgRhgAiAYB&sclient=gws-wiz
https://www.google.com/search?q=Heart+rate+during+deep+sleep&sa=X&sxsrf=APq-WBtbIsGJvOqnrsoLulxpV2Dn6ObPXg:1647597427519&tbm=isch&source=iu&ictx=1&vet=1&fir=d8U0AjdUn2Hq8M%252CJ6G2McglbWU10M%252C_%253BYujE32aEPqz_HM%252CF6wGCxQQL1or7M%252C_%253BC8JqlQs-JuNNiM%252C26Ena5o3dZMSbM%252C_%253B1vWr9HE4nfCQyM%252CwKg7ooAyva3HlM%252C_%253Bz8NNqlbPrDR5kM%252CJ6G2McglbWU10M%252C_%253BfWczVw7BsXfwhM%252CJ6G2McglbWU10M%252C_%253BbrWP2bRzR-wT8M%252C0kH5SmyKGoVcBM%252C_%253BxqVXA5D7rpCuzM%252CC2FxBIP0-WIB8M%252C_%253BcyZiV_N_3hM9HM%252CfccPuld-9hHSEM%252C_%253B8B_EU_aOOnj5uM%252CFoinBQ_Uet3FhM%252C_&usg=AI4_-kQAaGL_VBoXEUW2PM5km-rYvJPkkg&ved=2ahUKEwihpJnbss_2AhXOr1YBHd9YB8IQ9QF6BAgZEAE#imgrc=d8U0AjdUn2Hq8M
https://www.nature.com/articles/1300146
https://www.nature.com/articles/1300146#ref-CR1
https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3963245/
https://nyxo.app/what-can-heart-rate-tell-about-your-sleep#:~:text=In%20general%2C%20a%20low%20resting,lowest%20point%20of%20the%20curve.
https://www.sleepfoundation.org/stages-of-sleep/deep-sleep
https://www.healthline.com/health/how-much-deep-sleep-do-you-need#deep-sleep
https://web.telegram.org/k/
https://t.me/Our_1oveCommunity
https://myamazfit.ru/threads/bip-prilozhenija-dlja-bipos-elf.1174/page-67
https://github.com/AlexanderBaransky/Realtime_HR_Monitor4BipOS/blob/v2.0/main.c
https://myamazfit.ru/threads/bip-application-develop-for-bipos-sdk-en.1171/page-2#post-45389
https://myamazfit.ru/attachments/bip-png.26662/
https://nguyenvanhieu.vn/vong-lap-while-va-do-while-trong-c/
https://laptrinhcanban.com/c/lap-trinh-c-co-ban/vong-lap-trong-c/while-trong-c/
https://nguyenvanhieu.vn/vong-lap-for-trong-c/
https://laptrinhcanban.com/c/lap-trinh-c-co-ban/vong-lap-trong-c/for-trong-c/
https://cachhoc.net/2014/12/10/lap-trinh-c-bai-4-phep-toan-toan-tu-trong-c/


*/

void ketqua(){
	set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  load_font();
  set_hrm_mode(0); 	//	tắt màn hình nhịp tim để tiết kiệm pin (không hoạt động với độ trễ từ 0 - 10 giây)
  set_fg_color(COLOR_WHITE);
  text_out_center("Ket qua cu", 88, 5);
		text_out("Light", 10, 25); //min
		text_out("Deep", 10, 55); //max
		text_out("Tinh", 10, 85); //avg
		/*int abc;
		struct datetime_ dt;
		get_current_date_time(&dt);
									 abc = dt.min;
									char bcv[10];
		_sprintf(bcv, "%d", abc);
		set_fg_color(COLOR_WHITE);
		text_out_font(11, bcv, 120 , 85, 1);	*/						
		//Còn chỗ cho hiển thị giờ bắt đầu ở giữa tỉnh và tổng
         char clock_start[8]; 			//	текст время		12:34
		_sprintf(clock_start, "%02d:%02d %s", afhour, afmin, "-");
		set_fg_color(COLOR_WHITE);
		text_out_font(11, clock_start, 5 , 115, 1);
		char clock_end[8]; 			//	текст время		12:34
		_sprintf(clock_end, "%02d:%02d %s", edhour, edmin, ":");
		set_fg_color(COLOR_WHITE);
		text_out_font(11, clock_end, 60 , 115, 1);
		int timehour;
		if (edhour - afhour < 0) {
			timehour = edhour - afhour + 24;
		} else {
			timehour = edhour - afhour;
		}
		int timemin;
		if (edmin - afmin < 0) {
			timemin = edmin - afmin + 60;
			timehour = timehour - 1;
		} else {
			timemin = edmin - afmin;
		}
		char gio[10];
		_sprintf(gio, "%d", timehour * 60 + timemin);
		set_fg_color(COLOR_RED);
		text_out_font(11, gio, 115 , 115, 1);
		text_out("min", 148 , 115);
        //Hàm show_big_digit(Mã font, Đoạn code cần hiện, Trục x, Trục y, khoảng cách)
        //Hàm Text_out_font(Mã font, đoạn mã cần hiển thị, trục x, trục y, khoảng cách)		
		// ở đây chúng tôi thực hiện các hành động cần thiết nếu hàm được khởi chạy lần đầu tiên từ menu: điền vào tất cả các cấu trúc dữ liệu, v.v.
	
char x[10];
char y[10];
char z[10];
char q[10];	
		set_fg_color(COLOR_WHITE);
		_sprintf(y, "%d", h);	//Light
		text_out_font(1, y, 80, 25, 2);
        set_fg_color(COLOR_WHITE);
		_sprintf(z, "%d", j);	//Deep
		text_out_font(1, z, 80, 55, 2);
       set_fg_color(COLOR_WHITE);
	   _sprintf(q, "%d", k);	//Tinh
		text_out_font(1, q, 80, 85, 2);
			
		
		set_bg_color(COLOR_BLACK);
			set_fg_color(COLOR_WHITE);
		text_out("Tong:", 10, 150); //Tong
		
			
			set_bg_color(COLOR_BLACK);
		set_fg_color(COLOR_WHITE);
		_sprintf(x, "%d", l);	//Sleep
		text_out_font(10, x, 65, 150, 2);	
  set_update_period(1, 60000);
  repaint_screen_lines(0, 176); // обновляем строки экрана
}

void menu(){	//	 функция рисования экранов настройки
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

	app_data->menu_is_on = 1;

	set_bg_color(COLOR_BLACK);	// устанавливаем цвет фона
	set_fg_color(COLOR_WHITE);	// устанавливаем цвет текста
	fill_screen_bg();	//	заполняем экран этим цветом

	if (app_data->menu_stage == 0){
		text_out_center("Bật đèn nền?", 88, 65);	// выводим текст

		// рисуем кнопки "Да" и "Нет"
		set_fg_color(COLOR_RED);
		draw_filled_rect(0, 136, 88, 176);
		show_res_by_id(ICON_CANCEL_RED, 37, 149);

		set_fg_color(COLOR_GREEN);
		draw_filled_rect(88, 136, 176, 176);
		show_res_by_id(ICON_OK_GREEN, 125, 149);
	}  else if (app_data->menu_stage == 1){//Khoảng thời gian giữa 2 lần đo (giây)
		char text[10];
		_sprintf(text, "%d", app_data->seconds_between_rec);
		text_out_font(3, text, FIRST_MENU_BIG_DIGITS_COORD_X, FIRST_MENU_BIG_DIGITS_COORD_Y, 2);
	
		show_elf_res_by_id(app_data->proc->index_listed, 4, 30, 65);
		show_elf_res_by_id(app_data->proc->index_listed, 3, 110, 65);
	
		text_out_center("Khoảng cách đo", 88, 5);
		text_out_center("giữa 2 lần (giây)", 88, get_text_height() + 6);

		set_fg_color(COLOR_AQUA);
		draw_filled_rect(0, 136, 176, 176);
		show_res_by_id(317, 147, 147);
	}else if (app_data->menu_stage == 2){//Trên menu tổng kết kết quả
		vibrate(2, 300, 300);	// уведомляем пользователя об окончании измерения вибрацией
        
		text_out_center("Tổng hợp thời", 88, 5);
		text_out_center("gian ngủ (phút)", 88, get_text_height() + 5);
		text_out_center("Light", (text_width("Light") / 2) + 15, 2 * get_text_height() + 10); //min
		text_out_center("Deep", (176 - (text_width("Deep") / 2)) - 15, 2 * get_text_height() + 10); //max
		text_out_center("Tinh", 88, 2 * get_text_height() + 10); //avg
	

        //Hàm show_big_digit(Mã font, Đoạn code cần hiện, Trục x, Trục y, khoảng cách)
        //Hàm Text_out_font(Mã font, đoạn mã cần hiển thị, trục x, trục y, khoảng cách)		
		char min[10];
		_sprintf(min, "%d", find_min());
		text_out_font(3, min, 2, 3 * get_text_height() + 15, 2);

		char max[10];
		_sprintf(max, "%d", find_max());
		text_out_font(3, max, 120, 3 * get_text_height() + 15, 2);

		char avg[10];
		_sprintf(avg, "%d", find_avg());
		text_out_font(3, avg, 63, 3 * get_text_height() + 15, 2);
					struct datetime_ dt;
									get_current_date_time(&dt);
									 endenmin = dt.min;
									 endenhour = dt.hour;	
						//Còn chỗ cho hiển thị giờ bắt đầu ở giữa tỉnh và tổng
         char clock_start[8]; 			//	текст время		12:34
		_sprintf(clock_start, "%02d:%02d %s", anfanghour, anfangmin, "-");
		set_fg_color(COLOR_WHITE);
		text_out_font(11, clock_start, 5 , 115, 1);
		char clock_end[8]; 			//	текст время		12:34
		_sprintf(clock_end, "%02d:%02d %s", endenhour, endenmin, ":");
		set_fg_color(COLOR_WHITE);
		text_out_font(11, clock_end, 60 , 115, 1);
		int timehour;
		if (endenhour - anfanghour < 0) {
			timehour = endenhour - anfanghour + 24;
		} else {
			timehour = endenhour - anfanghour;
		}
		int timemin;
		if (endenmin - anfangmin < 0) {
			timemin = endenmin - anfangmin + 60;
			timehour = timehour - 1;
		} else {
			timemin = endenmin - anfangmin;
		}
		char gio[10];
		_sprintf(gio, "%d", timehour * 60 + timemin);
		set_fg_color(COLOR_RED);
		text_out_font(11, gio, 115 , 115, 1);
		text_out("min", 148 , 115);
	
		
		set_fg_color(COLOR_AQUA);
		draw_filled_rect(0, 136, 176, 176);
		show_res_by_id(317, 147, 147);
		
		set_fg_color(COLOR_BLACK);
		draw_filled_rect(0, 136, 95, 176);
		
		set_bg_color(COLOR_BLACK);
			set_fg_color(COLOR_WHITE);
		text_out("Tong:", 10, 150); //Tong
		
			char sum[10];
			set_bg_color(COLOR_BLACK);
		set_fg_color(COLOR_WHITE);
		_sprintf(sum, "%d", find_sum());
		text_out_font(10, sum, 65, 150, 2);
		update_settings();
	}
}

void redraw_screen(){	//	функция перерисовки графика после оверлейных экранов
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

  set_backlight_state(app_data->backlight);
	set_bg_color(COLOR_BLACK);
  fill_screen_bg();
 set_fg_color(COLOR_WHITE);
    draw_filled_rect_bg(70, 100, 102, 132);
   set_bg_color(COLOR_WHITE);
     set_fg_color(COLOR_BLACK);
  text_out("SLEEPING...", 80, 110); 

	// tính chỉ số của bản ghi đầu tiên trên màn hình hiện tại
	int i;
	if (app_data->rec_counter > app_data->rec_counter_per_screen){
		i = app_data->rec_counter - app_data->rec_counter_per_screen;
	} else {
		i = 0;	// если первый экран, то индекс 0
	}
	app_data->rec_counter_per_screen = 0;
	//	в цикле заново отрисовываем график, т. к. он стирается после оверлейного экрана.
	for (; i < app_data->rec_counter; i++){
		int heartrate = app_data->records[i];
		app_data->rec_counter_per_screen = app_data->rec_counter_per_screen + 1;
		repaint_screen_lines(0, 176);	//	обновление экрана
	}
	nap_job_wrapper();
	
	set_hrm_mode(0x20);	// bật máy theo dõi nhịp tim
}

//Dựa vào video quay lại cảnh Bin ngủ vào ban đêm cũng như app Mifit báo lại thì:
//Muốn phân biệt light và deep chỉ có thể dựa thêm đo hoạt động cơ thể mà thôi
//Deep sẽ nằm im luôn còn light thì không
/* Dựa vào nguyên lý trên nên Bin có phát hiện ra rằng
Khi cơ thể nằm im thì nhịp tim rất ít thay đổi (Chêch lệch cỡ 1- 2 nhịp) hay rất là đều luôn 
Lúc này sẽ lấy bình phương (Để trách bị trừ ra kết quả âm) của hiệu 2 nhịp tim kề nhau:
https://v1study.com/c-ham-toan-hoc-math-a75.html
Trong phần lớn các deep sleep thì trung bình cần khoảng thời gian ít nhất là 9 phút trở lên (Đã coi qua video)
Nghĩa là nếu cơ thể nằm im trong 9 phút trở lên là bắt đầu vào deep sleep
Vì Bin thấy phần lớn cơ thể của Bin khi ngủ cứ trung bình 7 phút là đổi tư thế rồi (trung bình là vậy), cũng như 
app Mifit cũng có dự báo sai
Nên dựa vào 2 kết quả trên Bin sẽ lấy giá trị trung bình là 10 phút
Cũng như khi thay đổi tư thế, diễn ra rất nhanh nên Bin đề xuất là đo nhịp tim 30 giây- 1 phút 1 lần là đẹp nhất 
*/

int find_min(){	//	функция поиска минимального значения в массиве
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

	//	в цикле перебираем массив и выбираем минимальное значение
	int t = app_data->seconds_between_rec;
	if(app_data->seconds_between_rec % 60 == 30 ){
		t = t + 30;
	}
	int min = 0; //Light
	int d = 0;
	int a = 0;
	/*
	for (int i = 0; i < app_data->rec_counter ; i++){
		if ( (app_data->records[i] >= 49) && (app_data->records[i] < 68) ){
			min = min + t/60 ;
			
			if ( (app_data->records[i] - app_data->records[i - 1] < 3 ) && (app_data->records[i] - app_data->records[i - 1] > -3 ) && (app_data->records[i] >= 49) && (app_data->records[i] < 68) ){
			a = a + t/60; 
			if (a > 9) {
			d = d + a ;
			}
			else { d = d + 0; }
		}
		else {
			a = 0;
		}
		} 
			}
	if ( d / min > 0.46) {
		min = min * (100 - 46) / 100;
	} else {
	min = min - d;
	} */
	if ( find_max() / find_sum() > 0.46) {
		min = find_sum() * (100 - 46) / 100;
	} else {
	min = find_sum() - find_max();
	}
	return min;	//	возвращаем минимальное значение
}

int find_max(){	//	функция поиска минимального значения в массиве
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

	//	в цикле перебираем массив и выбираем максимальное значение
	int t = app_data->seconds_between_rec;
	if(app_data->seconds_between_rec % 60 == 30 ){
		t = t + 30;
	}
	int max = 0; //Deep
	int a = 0;
	int d = 0;
	for (int i = 0; i < app_data->rec_counter ; i++){
		if ( (app_data->records[i] >= 35) && (app_data->records[i] < 68) ){
			d = d + t/60 ;
		if ( (app_data->records[i] - app_data->records[i - 1] < 3 ) && (app_data->records[i] - app_data->records[i - 1] > -3 ) && (app_data->records[i] < 68) ){
			a = a + t/60; 
			if (a > 9) {
			max = max + a ;
			}
			else { max = max + 0; }
		}
		else {
			a = 0;
		}
	}
	}
	if ( max / d > 0.46) {
		max = d * 46 / 100;
	} else {
		max = max;
	}
	return max;	//	возвращаем максимальное значение
}

int find_avg(){	//	функция поиска среднего арифметического значения в массиве
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

	//	в цикле перебираем массив и вычисляем сумму всех его элементов
	int t = app_data->seconds_between_rec;
	if(app_data->seconds_between_rec % 60 == 30 ){
		t = t + 30;
	}
	int avg = 0;  // Tỉnh
	for (int i = 0; i < app_data->rec_counter ; i++){
		if (app_data->records[i] > 68){
			avg = avg + t/60 ;
		}
	}
	return avg;	// trả về giá trị trung bình cộng
}

int find_sum(){	//	функция поиска среднего арифметического значения в массиве
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

	//	в цикле перебираем массив и вычисляем сумму всех его элементов
	int t = app_data->seconds_between_rec;
	if(app_data->seconds_between_rec % 60 == 30 ){
		t = t + 30;
	}
	int sum = 0;  // Tong
	for (int i = 0; i < app_data->rec_counter ; i++){
		if ( (app_data->records[i] < 68) && (app_data->records[i] >= 35) ){
			sum = sum + t/60 ;
		}
	}
	return sum;	// trả về tong
}

void update_settings(){
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана
struct settings_ settings;

	// đọc cài đặt từ bộ nhớ không bay hơi
	ElfReadSettings(app_data->proc->index_listed, &settings, 0, sizeof(settings));

	// записываем данные из структуры settings в app_data
	
	settings.seconds_between_rec = app_data->seconds_between_rec;
	settings.backlight = app_data->backlight;
					
        settings.nhe = find_min();
		settings.deep = find_max();
		settings.sleep = find_sum();
		settings.tinh = find_avg();
		
		settings.beginhour = anfanghour;
		settings.beginmin = anfangmin;
		settings.endhour = endenhour;
		settings.endmin = endenmin;
	
	// записываем настройки в энергонезависимую память
	ElfWriteSettings(app_data->proc->index_listed, &settings, 0, sizeof(settings));
}


int nap_job_wrapper(){	//	обертка для nap_job для исправления бага и преждевременным рисованием графика
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана
	
	if (app_data->menu_is_on == 0){
		nap_job();
	}
	return 0;
}

//Vẽ lại cấu trúc biểu đồ nếu cần
int nap_job(){
// nếu cần, bạn có thể sử dụng dữ liệu màn hình trong chức năng này
struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана


void* hrm_data = get_hrm_struct();	//	lấy cấu trúc dữ liệu theo dõi nhịp tim
int heartrate;

if (get_fw_version() == NOT_LATIN_1_1_2_05){	//	xác định phiên bản phần sụn để hoạt động với máy đo nhịp tim
	app_data->status = ((hrm_data_struct_legacy*)hrm_data)->ret_code;
	heartrate = ((hrm_data_struct_legacy*)hrm_data)->heart_rate;
} else {
	app_data->status = ((hrm_data_struct*)hrm_data)->ret_code;
	heartrate = ((hrm_data_struct*)hrm_data)->heart_rate;
}

set_hrm_mode(0x20);	// bật máy theo dõi nhịp tim
//Ở trên kiểm tra OKe

switch (app_data->status){	//	trạng thái cảm biến quá trình
	default:
	

	case 0:{		// đo lường hoàn tất thành công
// kiểm tra xem đường đồ thị đã đạt tỷ lệ chưa, nếu có thì bắt đầu vẽ đồ thị từ đầu màn hình
			app_data->rec_counter_per_screen = 0;		
		// điền vào cấu trúc với dữ liệu
		app_data->records[app_data->rec_counter] = heartrate;
		app_data->rec_counter = app_data->rec_counter + 1;
		app_data->rec_counter_per_screen = app_data->rec_counter_per_screen + 1;
	

		set_hrm_mode(0); 	//	tắt màn hình nhịp tim để tiết kiệm pin (không hoạt động với độ trễ từ 0 - 10 giây)
		//	выводим на экран минимальное и максимальное значения пульса
		draw_filled_rect_bg(STATS_COORD_X, STATS_COORD_Y, STATS_COORD_X + text_width("max:160"), 2 * STATS_COORD_Y + get_text_height());
		

		int time;
		if (app_data->seconds_between_rec == 0){
			time = 200;
		} else {
			time = app_data->seconds_between_rec * 1000;
		}
		set_update_period(1, time);	//	заново заводим таймер
		
		break;
	} 

	case 2:{		// đo lường nhịp tim không thành công
		vibrate(2, 100, 100);	//	предупреждаем пользователя вибрацией
		set_hrm_mode(0x20);	//	khởi động lại máy theo dõi nhịp tim
		set_update_period(1, 200);	//	заново заводим таймер
				break;
	}
	
}


// bảo vệ chống tràn mảng //rec_counter: Chức năng đếm dúng biến i
if (app_data->rec_counter == 9990){
	set_hrm_mode(0);
	set_update_period(0, 0);
	app_data->menu_stage = 4;
	show_menu_animate(menu, 0, ANIMATE_LEFT);
}
return 0;
}

void takenap(){
	set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  load_font();
  set_hrm_mode(0); 	//	tắt màn hình nhịp tim để tiết kiệm pin (không hoạt động với độ trễ từ 0 - 10 giây)
      show_statusbar(3, COLOR_BLACK,COLOR_WHITE);	// статус бар
	
	char crnd[8];     // переменная для перевода переменной col типа int в  тип char для печати                               
    _sprintf(crnd, "%07d", COLOR_WHITE); // конвертируем число int в char лидирующие нули, при необходимости, _sprintf добьёт сам
	set_bg_color(COLOR_BLACK); // делаем фон черным
	draw_filled_rect_bg(0, 43, 176, 110); // стираем предыдущее значение
		// отрисовка значений
		set_fg_color(COLOR_WHITE);
		text_out_center("When sleep, press", 88, 35);
		text_out_center("this button", 88, 55);
		text_out_center("By Tony", 88, 75);
 set_fg_color(COLOR_AQUA);
  draw_filled_rect_bg(70, 100, 102, 132);
   set_bg_color(COLOR_AQUA);
   set_fg_color(COLOR_BLACK);
  text_out("SLEEP", 80, 110);  
  set_update_period(1, 60000);
  repaint_screen_lines(0, 176); // обновляем строки экрана
};

void screen_job(){
  struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  struct app_data_ *app_data = *app_data_p;
  


 if (app_data->screen == 1) {
  draw_board();
repaint_screen_lines(0, 176);
set_update_period(1, 500);
 }
  else if ( app_data->screen == 2 ) {
	  draw_score_screen();
repaint_screen_lines(0, 176);
set_update_period(1, 60000);
  }
  else if ( app_data->screen == 6 ) {
ask_confirmation(); //Emergency
repaint_screen_lines(0, 176);
set_update_period(1, 500);
  }
    else if ( app_data->screen == 8 ) {
thegioi1();
repaint_screen_lines(0, 176);
set_update_period(1, 60000);
  }
        else if ( app_data->screen == 10 ) {
takenap();
repaint_screen_lines(0, 176);
set_update_period(1, 60000);
  }
          else if ( app_data->screen == 16 ) {
ketqua();
repaint_screen_lines(0, 176);
set_update_period(1, 600000);
  }
      else if ( app_data->screen == 9 ) {
thegioi2();
repaint_screen_lines(0, 176);
set_update_period(1, 60000);
  }
  else if ( app_data->screen == 7 ) {
	 chon_lua(); //Chonlua
repaint_screen_lines(0, 176);
set_update_period(1, 5000);
  }
    else if ( app_data->screen == 13 ) {
	tim();
repaint_screen_lines(0, 176);
set_update_period(1, 60000);
  }
      /*else if ( app_data->screen == 18 ) {
weather_screen();
repaint_screen_lines(0, 176);
set_update_period(1, 1200000);
  }*/
      else if ( app_data->screen == 14 ) {
	tim_cal();
repaint_screen_lines(0, 176);
set_update_period(1, 600000);
  }/*
        else if ( app_data->screen == 20 ) {
	weather1();
repaint_screen_lines(0, 176);
set_update_period(1, 600000);
  }
  else if ( app_data->screen == 21 ) {
	weather2();
repaint_screen_lines(0, 176);
set_update_period(1, 600000);
  }*/
  else if ( app_data->screen == 17 ) {
	weather();
repaint_screen_lines(0, 176);
set_update_period(1, 120000);
  }
    else if ( app_data->screen == 19 ) {
	weather_result();
repaint_screen_lines(0, 176);
set_update_period(1, 120000);
  }
      else if ( app_data->screen == 15 ) {
	 tim_result(); //Chonlua
repaint_screen_lines(0, 176);
set_update_period(1, 600000);
  }
        else if ( app_data->screen == 3 ) {
	  set_bg_color(COLOR_WHITE);
                                        fill_screen_bg();
									
                                        show_elf_res_by_id(app_data->proc->index_listed, 0, 2, 2);   //SSKDT
                                        set_bg_color(COLOR_WHITE);
                                        set_fg_color(COLOR_BLACK);
repaint_screen_lines(0, 176);
set_update_period(1, 600000);
  }
          else if ( app_data->screen == 4 ) {
	  set_bg_color(COLOR_WHITE);
                                        fill_screen_bg();
                                        show_elf_res_by_id(app_data->proc->index_listed, 1, 2, 2);  //PC COVID
                                        set_bg_color(COLOR_WHITE);
                                        set_fg_color(COLOR_BLACK);
repaint_screen_lines(0, 176);
set_update_period(1, 600000);
  }
  else if ( app_data->screen == 5 ) {
	   set_bg_color(COLOR_WHITE);
                                        fill_screen_bg();
                                        show_elf_res_by_id(app_data->proc->index_listed, 2, 0, 0);    //TONY
                                        set_bg_color(COLOR_WHITE);
                                        set_fg_color(COLOR_BLACK);
repaint_screen_lines(0, 176);
set_update_period(1, 600000);
  }
  else if ( app_data->screen == 11 ) {
	  redraw_screen();
	  repaint_screen_lines(0, 176);
  int time;
		if (app_data->seconds_between_rec == 0){
			time = 200;
		} else {
			time = app_data->seconds_between_rec * 1000;
		}
		set_update_period(1, time);	//	заново заводим таймер
  }
    else if ( app_data->screen == 18 ) {
	  weather_screen();
	  // lấy dữ liệu điều hướng
get_navi_data(&app_data->navi_data);

if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
	
	if (app_data->store_point_delayed){
		app_data->store_point_delayed = false;
		app_data->store_point_btn_visible = true;
			
	}
}
	  repaint_screen_lines(0, 176);
  int time;
		if (app_data->seconds_between_rec < 150){
			time = 120000;
		} else {
			time = app_data->seconds_between_rec * 1000;
		}
		set_update_period(1, time);	//	заново заводим таймер
  }
  else {

repaint_screen_lines(0, 176);
set_update_period(1, 60000);
  }
  
		  
}

/*
int takenap_cal(){
	struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  struct app_data_ *app_data = *app_data_p;
  set_backlight_state(0);
	set_bg_color(COLOR_BLACK);
  fill_screen_bg();
 set_bg_color(COLOR_WHITE);
  set_fg_color(COLOR_BLACK);
  draw_filled_rect_bg(70, 100, 102, 132);
   set_bg_color(COLOR_WHITE);
  text_out("SLEEPING...", 80, 110);  
  int T; //Tỉnh
  T = 0;
  int L; //Light Sleep
  L = 0;
  int D; //Deep Sleep
  D = 0;
    set_hrm_mode(0x20);	// bật máy theo dõi nhịp tim
	void* hrm_data = get_hrm_struct(); 
int heartrate;


if (get_fw_version() == NOT_LATIN_1_1_2_05){
	app_data->status = ((hrm_data_struct_legacy*)hrm_data)->ret_code;
	heartrate = ((hrm_data_struct_legacy*)hrm_data)->heart_rate;

} else {
	app_data->status = ((hrm_data_struct*)hrm_data)->ret_code;
	heartrate = ((hrm_data_struct*)hrm_data)->heart_rate;


}
		set_hrm_mode(0x20);	// bật máy theo dõi nhịp tim
		if (heartrate > 68){
	T = T + 1;
		}
	else if (heartrate >= 49) {
		L = L + 1;
	}
	else { D = D + 1; }
	
	return T;
	return L;
	return D;
	set_update_period(1, 60000);
  repaint_screen_lines(0, 176); // обновляем строки экрана
};

/*
int find_Tinh(){	//	функция поиска минимального значения в массиве
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

	//	в цикле перебираем массив и выбираем максимальное значение
	int Tinh;
	Tinh = 0;
	for (int i = 0; i < 480; i++){
		if (( app_data->records[i] > 68) ){
			Tinh = Tinh + 1;
		}
	}
	return Tinh;	//	возвращаем максимальное значение
}

int find_Light(){	//	функция поиска минимального значения в массиве
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

	//	в цикле перебираем массив и выбираем максимальное значение
	int Light;
	Light = 0;
	for (int i = 0; i < 480; i++){
		if ( (app_data->records[i] >= 49) && ( app_data->records[i] < 68) ){
			Light = Light + 1;
		}
	}
	return Light;	//	возвращаем максимальное значение
}

int find_Deep(){	//	функция поиска минимального значения в массиве
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

	//	в цикле перебираем массив и выбираем максимальное значение
	int Deep;
	Deep = 0;
	for (int i = 0; i < 480; i++){
		if (( app_data->records[i] < 49) ){
			Deep = Deep + 1;
		}
	}
	return Deep;	//	возвращаем максимальное значение
}

void takenap_result(){
	set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  set_hrm_mode(0); 	//	tắt màn hình nhịp tim để tiết kiệm pin (không hoạt động với độ trễ từ 0 - 10 giây)
  set_backlight_state(1);
  load_font();
		// отрисовка значений
		show_statusbar(3, COLOR_BLACK,COLOR_WHITE);	// статус бар
	
	char crnd[8];     // переменная для перевода переменной col типа int в  тип char для печати                               
    _sprintf(crnd, "%07d", COLOR_WHITE); // конвертируем число int в char лидирующие нули, при необходимости, _sprintf добьёт сам
	set_bg_color(COLOR_BLACK); // делаем фон черным
	draw_filled_rect_bg(0, 43, 176, 110); // стираем предыдущее значение
		// отрисовка значений
		set_fg_color(COLOR_WHITE);
		text_out_center("Ket qua ngu (phut)", 88, 35);
		 /*
		 char Tinh[10];
	_sprintf(Tinh, "%s%d", "Tỉnh (phút):", find_Tinh());
	text_out(Tinh, 5, 55);
	char Light[10];
	_sprintf(Light, "%s%d", "Light Sleep (phút):", find_Light());
	text_out(Light, 5, 75);
	char Deep[10];
	_sprintf(Deep, "%s%d", "Deep Sleep (phút):", find_Deep());
	text_out(Deep, 5, 95);

	int a[4];
	_sprintf(a, "%d", T );
	text_out("Tỉnh: ", 5, 55);
	text_out(a, 55, 55);
	int b[4];
	_sprintf(b, "%d", L );
	text_out("Light: ", 5, 75);
	text_out(b, 55, 75);
	int c[4];
	_sprintf(c, "%d", D );
	text_out("Deep: ", 5, 95);
	text_out(c, 55, 95);
	set_update_period(1, 60000);
  repaint_screen_lines(0, 176); // обновляем строки экрана
};
*/

/*
Dự báo thời tiết dựa vào thay đổi của barometer
Dự án này nghiên cứu lại sau

void weather(){
	
	set_update_period(1, 60000);
  repaint_screen_lines(0, 176); // обновляем строки экрана
};
*/

void weather(){
		struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  struct app_data_ *app_data = *app_data_p;
	set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  load_font();
       show_statusbar(3, COLOR_BLACK,COLOR_WHITE);	// статус бар
	
	char crnd[8];     // переменная для перевода переменной col типа int в  тип char для печати                               
    _sprintf(crnd, "%07d", COLOR_WHITE); // конвертируем число int в char лидирующие нули, при необходимости, _sprintf добьёт сам
	set_bg_color(COLOR_BLACK); // делаем фон черным
	draw_filled_rect_bg(0, 43, 176, 110); // стираем предыдущее значение
		// отрисовка значений
		set_fg_color(COLOR_WHITE);
		text_out_center("Du bao thoi tiet", 88, 35);
		text_out_center("By Tony", 88, 55);
		text_out_center("Luu y", 88, 75); 
		set_fg_color(COLOR_WHITE);
		text_out("App nay nang", 10, 95); 
		text_out("Dung 1 lan, reset", 10, 115); 
  set_bg_color(COLOR_AQUA);
  set_fg_color(COLOR_BLACK);
  draw_filled_rect_bg(70, 135, 102, 160);
   set_bg_color(COLOR_AQUA);
    set_fg_color(COLOR_BLACK);
  text_out("Bat dau", 75, 140); 
    
switch_gps_pressure_sensors(SENSOR_ENABLE);
app_data->sensor_mode = SENSOR_ENABLE;
// lấy dữ liệu điều hướng
get_navi_data(&app_data->navi_data);

if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
	
	if (app_data->store_point_delayed){
		app_data->store_point_delayed = false;
		app_data->store_point_btn_visible = true;
	}
}

	
	//	в цикле заново отрисовываем график, т. к. он стирается после оверлейного экрана.
	/*for (; i < app_data->weather_dem; i++){
		int app_data->navi_data.pressure = app_data->weather[i];
		app_data->weather_dem_man = app_data->weather_dem_man + 1;
		repaint_screen_lines(0, 176);	//	обновление экрана
	}
weather_job();*/
  
  
 set_update_period(1, 60000);
    repaint_screen_lines(0, 176); // обновляем строки экрана
};


void weather_screen(){	//	функция перерисовки графика после оверлейных экранов
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

  set_backlight_state(0);
	set_bg_color(COLOR_BLACK);
  fill_screen_bg();
 set_fg_color(COLOR_WHITE);
  text_out_center("DOI IT NHAT", 88, 110);  
  text_out_center("6 phut", 88, 130);  
  text_out_center("3 chu kỳ", 88, 150);  

	// tính chỉ số của bản ghi đầu tiên trên màn hình hiện tại
	// tính chỉ số của bản ghi đầu tiên trên màn hình hiện tại
	int i;
	if (app_data->weather_dem > app_data->weather_dem_man){
		i = app_data->weather_dem - app_data->weather_dem_man;
	} else {
		i = 0;	// если первый экран, то индекс 0
	}
	app_data->weather_dem_man = 0;
	//	в цикле заново отрисовываем график, т. к. он стирается после оверлейного экрана.
	for (; i < app_data->weather_dem; i++){
		app_data->navi_data.pressure = app_data->weather[i];
		app_data->weather_dem_man = app_data->weather_dem_man + 1;
		repaint_screen_lines(0, 176);	//	обновление экрана
}
	//	в цикле заново отрисовываем график, т. к. он стирается после оверлейного экрана.
	char text[10];
	_sprintf(text, "%d", app_data->weather[app_data->weather_dem]);
	text_out_font(10, text, BIG_DIGITS_COORD_X, 40, 2);	//	отображение цифр большим шрифтом
	weather_job();
switch_gps_pressure_sensors(SENSOR_ENABLE);
app_data->sensor_mode = SENSOR_ENABLE;
										// lấy dữ liệu điều hướng
												get_navi_data(&app_data->navi_data);

												if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
													
													if (app_data->store_point_delayed){
														
														app_data->store_point_delayed = false;
														app_data->store_point_btn_visible = true;
														
														
													}
												}
}


/*
Nhận xét: nếu với kiểu cấu trúc tách riêng từng cái như này thì sẽ 
nhận được dữ liệu riêng biệt luôn cũng như là sau 1 khoảng thời gian sẽ
được tự động update lại dữ liệu áp suất luôn

void weather1(){	//	функция поиска минимального значения в массиве
	struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  struct app_data_ *app_data = *app_data_p;
 struct res_params_ res_params;							// tham số tài nguyên đồ họa
						set_backlight_state(0);
	set_bg_color(COLOR_BLACK);
  fill_screen_bg();
						switch_gps_pressure_sensors(SENSOR_ENABLE);
app_data->sensor_mode = SENSOR_ENABLE;
char gps_state = SENSOR_GPS_DISABLED;					// trạng thái hiện tại của cảm biến

char lat[16]; 			//	текст широта  	N 012.345678_
char lon[16]; 			//	текст долгота 	E 012.345678_
char alt[16]; 			//	текст высота  	10000.00_м_
char prs[16]; 			//	текст давление	1123.45 гПа_
char prs1[16];
char tim[8]; 			//	текст время		12:34_


int pressure1 = 0;
//int pressure2 = 0;

/*
if (app_data->sensor_mode == SENSOR_ENABLE){	
	if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
		gps_state = SENSOR_GPS_FIXED;
	} else {
		gps_state = SENSOR_GPS_NOT_FIXED;
	}
} else {
	gps_state = SENSOR_GPS_DISABLED;
}

 

struct datetime_ dt;
get_current_date_time(&dt);
int a = dt.min;
switch_gps_pressure_sensors(SENSOR_ENABLE);
app_data->sensor_mode = SENSOR_ENABLE;
// lấy dữ liệu điều hướng
get_navi_data(&app_data->navi_data);

if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
	
	if (app_data->store_point_delayed){
		app_data->store_point_delayed = false;
		app_data->store_point_btn_visible = true;
	}
}

  if (IS_NAVI_PRESS_READY(app_data->navi_data.ready)){
		
				pressure1 =  app_data->navi_data.pressure;
				
				


}
shut_down_navi();
char prsa[16]; 
  set_fg_color(COLOR_WHITE);
  text_out("Lan 1", 80, 90);  
  _sprintf(prsa, "%.2d %s", pressure1, "hPa");
text_out(prsa, 10, 120);  
return pressure1;

};

void weather2(){	//	функция поиска минимального значения в массиве
	struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  struct app_data_ *app_data = *app_data_p;
 struct res_params_ res_params;							// tham số tài nguyên đồ họa
							set_backlight_state(0);
	set_bg_color(COLOR_BLACK);
  fill_screen_bg();
						switch_gps_pressure_sensors(SENSOR_ENABLE);
app_data->sensor_mode = SENSOR_ENABLE;
char gps_state = SENSOR_GPS_DISABLED;					// trạng thái hiện tại của cảm biến

/*
char lat[16]; 			//	текст широта  	N 012.345678_
char lon[16]; 			//	текст долгота 	E 012.345678_
char alt[16]; 			//	текст высота  	10000.00_м_
char prs[16]; 			//	текст давление	1123.45 гПа_
char prs1[16];
char tim[8]; 			//	текст время		12:34_


int pressure1 = 0;

int pressure2 = 0;

/*
if (app_data->sensor_mode == SENSOR_ENABLE){	
	if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
		gps_state = SENSOR_GPS_FIXED;
	} else {
		gps_state = SENSOR_GPS_NOT_FIXED;
	}
} else {
	gps_state = SENSOR_GPS_DISABLED;
}

 

struct datetime_ dt;
get_current_date_time(&dt);
int a = dt.min;
switch_gps_pressure_sensors(SENSOR_ENABLE);
app_data->sensor_mode = SENSOR_ENABLE;
// lấy dữ liệu điều hướng
get_navi_data(&app_data->navi_data);

if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
	
	if (app_data->store_point_delayed){
		app_data->store_point_delayed = false;
		app_data->store_point_btn_visible = true;
	}
}

  if (IS_NAVI_PRESS_READY(app_data->navi_data.ready)){
		
				pressure2 =  app_data->navi_data.pressure;
				
				


}
shut_down_navi();
char prsb[16]; 
  set_fg_color(COLOR_WHITE);
  text_out("Lan 2", 80, 90);  
  _sprintf(prsb, "%.2d %s", pressure2, "hPa");
text_out(prsb, 10, 120);
return pressure2;
};


void weather_result(){	//	функция поиска минимального значения в массиве
	struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  struct app_data_ *app_data = *app_data_p;
 
 set_backlight_state(1);

int u;
u = pressure2 - pressure1;
int k1;
k1 = pressure1;
int k2;
k2 = pressure2;
 
 
 char ta;
int t;
 switch_gps_pressure_sensors(SENSOR_ENABLE);
app_data->sensor_mode = SENSOR_ENABLE;
// lấy dữ liệu điều hướng
struct datetime_ dt;
get_current_date_time(&dt);
int a = dt.min;
switch_gps_pressure_sensors(SENSOR_ENABLE);
app_data->sensor_mode = SENSOR_ENABLE;
// lấy dữ liệu điều hướng
get_navi_data(&app_data->navi_data);

if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
	
	if (app_data->store_point_delayed){
		app_data->store_point_delayed = false;
		app_data->store_point_btn_visible = true;
	}
}

  if (IS_NAVI_PRESS_READY(app_data->navi_data.ready)){
t = app_data->navi_data.pressure / 100;
}
set_fg_color(COLOR_WHITE);
text_out("Test",  10, 140);
_sprintf(ta, "%.2d %s", t, "hPa");
set_fg_color(COLOR_WHITE);
text_out(ta, 60, 140);
 
 /*
  switch (gps_state){
		default:
		case SENSOR_GPS_DISABLED:{
			// vẽ biểu tượng GPS
			set_fg_color(COLOR_YELLOW);
			text_out("GPS Bi tat",  10, 20);
			
			break;
		}
		case SENSOR_GPS_NOT_FIXED:{
			//	отрисовка значка GPS
			set_fg_color(COLOR_RED);
			text_out("GPS Bi loi",  10, 20);
			
			break;
		}
  case SENSOR_GPS_FIXED:{ }
			if ( ( u > 7514 ) || ( u < -7514 ) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Có thể mưa",  10, 20);
				text_out("trong 36 giờ",  10, 40);
				text_out("Trời ấm lên",  10, 60);
		}
		else if ( (( u < 7514 ) && ( u > 7473 ))  || (( u > - 7514 ) && ( u < - 7473 )) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Có thể mưa",  10, 20);
				text_out("trong 24 giờ",  10, 40);
				text_out("Trời ấm lên",  10, 60);
		}
		else if ( (( u > 7423 ) && ( u < 7473 ))  || (( u > - 7473 ) && ( u < - 7423 )) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Có thể mưa",  10, 20);
				text_out("trong 18 giờ",  10, 40);
				text_out("Trời lạnh đi",  10, 60);
		}
		else if ( ( u < 7423 ) || ( u > -7423 ) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Trời tạnh",  10, 20);
				
		}
			
char prs[16]; 			//	текст давление	1123.45 гПа_
char prs1[16];		
char prs2[16];	
_sprintf(prs2, "%.2d", u);
text_out(prs, 80, 140);		
set_fg_color(COLOR_AQUA);
text_out("Ap suat 1 va 2 (hPa)", 10, 80);
_sprintf(prs, "%.2d %s", k1, "hPa");
text_out(prs, 10, 100);
_sprintf(prs1, "%.2d %s", k2, "hPa");
text_out(prs1, 10, 120);
		

  
 set_update_period(1, 120000);
    repaint_screen_lines(0, 176); // обновляем строки экрана

}; */

int shut_down_navi(){
struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана


	switch_gps_pressure_sensors(SENSOR_DISABLE);	//	отключение датчиков
	app_data->sensor_mode = SENSOR_DISABLE;
	
	return 0;		//	0 - thành công, 1 - không thành công
}


void weather_result(){	//	функция поиска минимального значения в массиве
	//struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  //struct app_data_ *app_data = *app_data_p;
 vibrate(2, 300, 300);	// уведомляем пользователя об окончании измерения вибрацией
 set_backlight_state(1);
 /*
char gps_state = SENSOR_GPS_DISABLED;					// trạng thái hiện tại của cảm biến

char lat[16]; 			//	текст широта  	N 012.345678_
char lon[16]; 			//	текст долгота 	E 012.345678_
char alt[16]; 			//	текст высота  	10000.00_м_

char tim[8]; 			//	текст время		12:34_


int pressure1 = 0;
int pressure2 = 0;


if (app_data->sensor_mode == SENSOR_ENABLE){	
	if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
		gps_state = SENSOR_GPS_FIXED;
	} else {
		gps_state = SENSOR_GPS_NOT_FIXED;
	}
} else {
	gps_state = SENSOR_GPS_DISABLED;
}

 

struct datetime_ dt;
get_current_date_time(&dt);
int a = dt.min;
switch_gps_pressure_sensors(SENSOR_ENABLE);
app_data->sensor_mode = SENSOR_ENABLE;
// lấy dữ liệu điều hướng
get_navi_data(&app_data->navi_data);

if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
	
	if (app_data->store_point_delayed){
		app_data->store_point_delayed = false;
		app_data->store_point_btn_visible = true;
	}
}

  if (IS_NAVI_PRESS_READY(app_data->navi_data.ready)){
		
				pressure1 =  app_data->navi_data.pressure;
				
				


}
if (dt.min == a + 2){
switch_gps_pressure_sensors(SENSOR_ENABLE);
app_data->sensor_mode = SENSOR_ENABLE;
// lấy dữ liệu điều hướng
get_navi_data(&app_data->navi_data);

if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
	
	if (app_data->store_point_delayed){
		app_data->store_point_delayed = false;
		app_data->store_point_btn_visible = true;
	}
}

  if (IS_NAVI_PRESS_READY(app_data->navi_data.ready)){
		
				pressure2 =  app_data->navi_data.pressure;
				
				


}
}
*/
int s;
s = pressure_1() ;

char prs2[10];
int u;
u = find_pressure();
_sprintf(prs2, "%d", find_pressure());
set_fg_color(COLOR_RED);
text_out(prs2, 90, 100);
    if ( s > 102269 ) {
	if ( ( u > 27091 ) || ( u < -27091 ) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Có thể mưa",  10, 20);
				text_out("trong 36 giờ",  10, 40);
				text_out("Trời ấm lên",  10, 60);
		}
		else if ( (( u < 27091 ) && ( u > 6773 ))  || (( u > - 27091 ) && ( u < - 6773 )) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Trời trong, lạnh",  10, 20);
				text_out("sau đó ấm",  10, 40);
				text_out("và có mưa",  10, 60);
		}
		else if ( ( u < 6773 ) || ( u > -6773 ) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Không có gì",  10, 20);
				text_out("thay đổi",  10, 40);
			
				
		} else {
		text_out("Trời tạnh",  10, 20);
		text_out("Chưa phát hiện",  10, 40);
		text_out("thay đổi gì",  10, 60);
		}	
	} else if ( ( s > 101592 ) && ( s < 102269 ) ) {
		if ( u < -27091 )  {
				set_fg_color(COLOR_WHITE);
				text_out("Có thể mưa",  10, 20);
				text_out("trong 24 giờ tới",  10, 40);
				text_out("Trời ấm lên",  10, 60);
		}
		else if ( u > 27091 ) {
				set_fg_color(COLOR_WHITE);
				text_out("Trời sẽ ấm",  10, 20);
				text_out("và có mưa",  10, 40);
				text_out("trong 2 ngày tới",  10, 60);
		}
		else if ( ( u < 6773 ) || ( u > -6773 ) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Thay đổi 1 chút",  10, 20);
				text_out("về nhiệt độ",  10, 40);
				text_out("trong 1, 2 ngày tới",  10, 60);
			
				
		} else {
		text_out("Trời tạnh",  10, 20);
		text_out("Chưa phát hiện",  10, 40);
		text_out("thay đổi gì",  10, 60);
		}	
	} else if ( ( s > 100914 ) && ( s < 101592 ) ) {
		if ( ( u > - 27091 ) && ( u < - 6773 ) )  { //Failing Slowly
				set_fg_color(COLOR_WHITE);
				text_out("Có thể mưa trong 18",  10, 20);
				text_out("giờ tới và sẽ tiếp",  10, 40);
				text_out("tục trong 1,2 ngày nữa",  10, 60);
		}
		else if ( u < -27091 ) { //Faling rapidly
				set_fg_color(COLOR_WHITE);
				text_out("Mưa và gió lớn",  10, 20);
				text_out("Sau đó 1,2 ngày",  10, 40);
				text_out("trời trong, lạnh",  10, 60);
		}
		else if ( u > 6773 ) {
				set_fg_color(COLOR_WHITE);
				text_out("Trời trong, lạnh",  10, 20);
				text_out("trong 12 giờ tới",  10, 40);
							
		} else {
		text_out("Trời tạnh",  10, 20);
		text_out("Chưa phát hiện",  10, 40);
		text_out("thay đổi gì",  10, 60);
		}	
	} else if ( s < 101592 ) {
		if ( ( u > - 27091 ) && ( u < - 6773 ) )  { //Failing Slowly
				set_fg_color(COLOR_WHITE);
				text_out("Mưa lớn. Nếu là ",  10, 20);
				text_out("mùa đông, sẽ có",  10, 40);
				text_out("đợt khí lạnh",  10, 60);
		}
		else if ( u < -27091 ) { //Faling rapidly
				set_fg_color(COLOR_WHITE);
				text_out("Có thể có bão to. Nếu",  10, 20);
				text_out("là mùa đông sẽ có đợt",  10, 40);
				text_out("lạnh trong 24 giờ tới",  10, 60);
		}
		else if ( u > 27091 ) {
				set_fg_color(COLOR_WHITE);
				text_out("Trời trong, lạnh",  10, 20);
		}else {
		text_out("Trời tạnh",  10, 20);
		text_out("Chưa phát hiện",  10, 40);
		text_out("thay đổi gì",  10, 60);
		}			
		
	} else {
		text_out("Trời tạnh",  10, 20);
		text_out("Chưa phát hiện",  10, 40);
		text_out("thay đổi gì",  10, 60);
		}	
	
			
char prs[10]; 			//	текст давление	1123.45 гПа_
char prs1[10];	
char temp[10];		
set_fg_color(COLOR_AQUA);
text_out("Ap suat 1 va 2 (hPa)", 10, 80);
_sprintf(prs, "%d %s", pressure_1(), "hPa");
text_out(prs, 10, 100);
_sprintf(prs1, "%d %s", pressure_2(), "hPa");
text_out(prs1, 10, 120);
set_fg_color(COLOR_GREEN);
text_out("Nhiet do gia- C", 10, 140);
_sprintf(temp, "%d", pressure_1()*75006/100000 - 760 + 23);
text_out(temp, 140, 140);
shut_down_navi();
		

  
 set_update_period(1, 120000);
    repaint_screen_lines(0, 176); // обновляем строки экрана

}

int find_pressure(){	//	функция поиска среднего арифметического значения в массиве
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

	//	в цикле перебираем массив и вычисляем сумму всех его элементов
	 int b;
	for (int i = 1; i < 2 ; i++){
		 b = app_data->weather[i + 1] - app_data->weather[i];

	}
	return b;	// trả về tong
}


int pressure_1(){
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

	//	в цикле перебираем массив и вычисляем сумму всех его элементов
	int p1;
	for (int i = 1; i < 2 ; i++){
		 p1 = app_data->weather[i] /100;

	}
	return p1;
	
}

int pressure_2(){
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

	//	в цикле перебираем массив и вычисляем сумму всех его элементов
	int p2;
	for (int i = 2; i < 3 ; i++){
		 p2 = app_data->weather[i] / 100;

	}
	return p2;
	
}

/*
int high1(){
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
	struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

	//	в цикле перебираем массив и вычисляем сумму всех его элементов
	int h;
	for (int i = 2; i < 3 ; i++){
		 h = app_data->weather_cao[i];

	}
	return h;
	
}*/


//Vẽ lại cấu trúc biểu đồ nếu cần
int weather_job(){
// nếu cần, bạn có thể sử dụng dữ liệu màn hình trong chức năng này
struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана

switch_gps_pressure_sensors(SENSOR_ENABLE);
	app_data->sensor_mode = SENSOR_ENABLE;
// lấy dữ liệu điều hướng
		get_navi_data(&app_data->navi_data);

		if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
			
			if (app_data->store_point_delayed){
				
				app_data->store_point_delayed = false;
				app_data->store_point_btn_visible = true;
				
				
			}
		}

	

		// đo lường hoàn tất thành công
// kiểm tra xem đường đồ thị đã đạt tỷ lệ chưa, nếu có thì bắt đầu vẽ đồ thị từ đầu màn hình
			app_data->weather_dem_man = 0;	
		
		// điền vào cấu trúc với dữ liệu
		char text[10];
		if (IS_NAVI_PRESS_READY(app_data->navi_data.ready)){
		app_data->weather[app_data->weather_dem] = app_data->navi_data.pressure;
		}
		if ((int) app_data->options.press_cal){
		app_data->weather[app_data->weather_dem] *= app_data->options.press_cal;
	}
	/*if (IS_NAVI_PRESS_READY(app_data->navi_data.ready)){
		app_data->weather_cao[app_data->weather_dem_cao] = app_data->navi_data.altitude;
		}
		if ((int) app_data->options.press_cal){
		app_data->weather_cao[app_data->weather_dem_cao] *= app_data->options.press_cal;
	}*/

	_sprintf(text, "%d %s", app_data->weather[app_data->weather_dem], "hPa");
		//app_data->weather_dem_cao = app_data->weather_dem_cao + 1;
		app_data->weather_dem = app_data->weather_dem + 1;
		app_data->weather_dem_man = app_data->weather_dem_man + 1;
		set_fg_color(COLOR_WHITE);
		
		_sprintf(text, "%d", app_data->navi_data.pressure);
text_out_font(10, text, BIG_DIGITS_COORD_X, 40, 2);	//	отображение цифр большим шрифтом
		
		draw_filled_rect_bg(STATS_COORD_X, STATS_COORD_Y, STATS_COORD_X + text_width("max:160"), 2 * STATS_COORD_Y + get_text_height());
		

		int time;
		if (app_data->seconds_between_rec < 150){
			time = 120000;
		} else {
			time = app_data->seconds_between_rec * 1000;
		}
		set_update_period(1, time);	//	заново заводим таймер
  repaint_screen_lines(0, 176);
		
	
	



// bảo vệ chống tràn mảng //rec_counter: Chức năng đếm dúng biến i
if (app_data->rec_counter == 10){
	shut_down_navi();
	weather_result();
	set_update_period(0, 0);
	show_menu_animate(menu, 0, ANIMATE_LEFT);
	repaint_screen_lines(0, 176);
}
repaint_screen_lines(0, 176);
return 0;
}

/* Code mới mà vì bảng này không có bao hàm được hết nên Bin để đây tham khảo
if ( s > 102269 ) {
	if ( ( u > 27091 ) || ( u < -27091 ) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Có thể mưa",  10, 20);
				text_out("trong 36 giờ",  10, 40);
				text_out("Trời ấm lên",  10, 60);
		}
		else if ( (( u < 27091 ) && ( u > 6773 ))  || (( u > - 27091 ) && ( u < - 6773 )) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Trời trong, lạnh",  10, 20);
				text_out("sau đó ấm",  10, 40);
				text_out("và có mưa",  10, 60);
		}
		else if ( ( u < 6773 ) || ( u > -6773 ) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Không có gì",  10, 20);
				text_out("thay đổi",  10, 40);
			
				
		}
	} else if ( ( s > 101592 ) && ( s < 102269 ) ) {
		if ( ( u > 27091 ) || ( u < -27091 ) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Có thể trời",  10, 20);
				text_out("ấm và mưa",  10, 40);
				text_out("Trong 2 ngày tới",  10, 60);
		}
		else if ( (( u < 27091 ) && ( u > 6773 ))  || (( u > - 27091 ) && ( u < - 6773 )) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Trời trong, lạnh",  10, 20);
				text_out("sau đó ấm",  10, 40);
				text_out("và có mưa",  10, 60);
		}
		else if ( ( u < 6773 ) || ( u > -6773 ) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Thay đổi 1 chút",  10, 20);
				text_out("về nhiệt độ",  10, 40);
				text_out("trong 1, 2 ngày tới",  10, 60);
			
				
		}
	}
	*/


/* Lúc đầu là Bin làm như này nhưng lúc sau phát hiện ra là hiểu sai nên sửa lại như trên
void weather_result(){	//	функция поиска минимального значения в массиве
	//struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  //struct app_data_ *app_data = *app_data_p;
 vibrate(2, 300, 300);	// уведомляем пользователя об окончании измерения вибрацией
 set_backlight_state(1);
 /*
char gps_state = SENSOR_GPS_DISABLED;					// trạng thái hiện tại của cảm biến

char lat[16]; 			//	текст широта  	N 012.345678_
char lon[16]; 			//	текст долгота 	E 012.345678_
char alt[16]; 			//	текст высота  	10000.00_м_

char tim[8]; 			//	текст время		12:34_


int pressure1 = 0;
int pressure2 = 0;


if (app_data->sensor_mode == SENSOR_ENABLE){	
	if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
		gps_state = SENSOR_GPS_FIXED;
	} else {
		gps_state = SENSOR_GPS_NOT_FIXED;
	}
} else {
	gps_state = SENSOR_GPS_DISABLED;
}

 

struct datetime_ dt;
get_current_date_time(&dt);
int a = dt.min;
switch_gps_pressure_sensors(SENSOR_ENABLE);
app_data->sensor_mode = SENSOR_ENABLE;
// lấy dữ liệu điều hướng
get_navi_data(&app_data->navi_data);

if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
	
	if (app_data->store_point_delayed){
		app_data->store_point_delayed = false;
		app_data->store_point_btn_visible = true;
	}
}

  if (IS_NAVI_PRESS_READY(app_data->navi_data.ready)){
		
				pressure1 =  app_data->navi_data.pressure;
				
				


}
if (dt.min == a + 2){
switch_gps_pressure_sensors(SENSOR_ENABLE);
app_data->sensor_mode = SENSOR_ENABLE;
// lấy dữ liệu điều hướng
get_navi_data(&app_data->navi_data);

if (IS_NAVI_GPS_READY(app_data->navi_data.ready)){
	
	if (app_data->store_point_delayed){
		app_data->store_point_delayed = false;
		app_data->store_point_btn_visible = true;
	}
}

  if (IS_NAVI_PRESS_READY(app_data->navi_data.ready)){
		
				pressure2 =  app_data->navi_data.pressure;
				
				


}
}


char prs2[10];
int u;
u = find_pressure();
_sprintf(prs2, "%d", find_pressure());
set_fg_color(COLOR_RED);
text_out(prs2, 90, 100);
 
	if ( ( u > 7514 ) || ( u < -7514 ) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Có thể mưa",  10, 20);
				text_out("trong 36 giờ",  10, 40);
				text_out("Trời ấm lên",  10, 60);
		}
		else if ( (( u < 7514 ) && ( u > 7473 ))  || (( u > - 7514 ) && ( u < - 7473 )) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Có thể mưa",  10, 20);
				text_out("trong 24 giờ",  10, 40);
				text_out("Trời ấm lên",  10, 60);
		}
		else if ( (( u > 7423 ) && ( u < 7473 ))  || (( u > - 7473 ) && ( u < - 7423 )) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Có thể mưa",  10, 20);
				text_out("trong 18 giờ",  10, 40);
				text_out("Trời lạnh đi",  10, 60);
		}
		else if ( ( u < 7423 ) || ( u > -7423 ) ) {
				set_fg_color(COLOR_WHITE);
				text_out("Trời tạnh",  10, 20);
				
		}
			
char prs[10]; 			//	текст давление	1123.45 гПа_
char prs1[10];			
set_fg_color(COLOR_AQUA);
text_out("Ap suat 1 va 2 (hPa)", 10, 80);
_sprintf(prs, "%d %s", pressure_1(), "hPa");
text_out(prs, 10, 100);
_sprintf(prs1, "%d %s", pressure_2(), "hPa");
text_out(prs1, 10, 120);
shut_down_navi();
		

  
 set_update_period(1, 120000);
    repaint_screen_lines(0, 176); // обновляем строки экрана

};*/


/*
http://hyperphysics.phy-astr.gsu.edu/hbase/Kinetic/barfor.html
https://www.nationalgeographic.org/encyclopedia/barometer/print/
https://sciencing.com/calculate-tons-cooling-cooling-tower-10058467.html
https://keisan.casio.com/exec/system/1224562962
https://www.google.com.vn/search?q=calculate+temperature+from+barometer&ie=UTF-8&oe=UTF-8&hl=en-vn&client=safari
http://www.csgnetwork.com/barcorrecttcalc.html
https://www.google.com.vn/search?q=in+hg+to+mmhg&ie=UTF-8&oe=UTF-8&hl=en-vn&client=safari
https://scied.ucar.edu/learning-zone/how-weather-works/highs-and-lows-air-pressure
https://keisan.casio.com/exec/system/1224562962
https://www.google.com/search?sxsrf=APq-WBvKCgJBSq709xh7dSr0xxzt2tsd8w:1649070953729&q=Air+pressure+at+temperature+calculator&sa=X&ved=2ahUKEwjr85-DpPr2AhUzNKYKHa8vAdgQ1QJ6BAgiEAE
https://www.translatorscafe.com/unit-converter/en-US/calculator/altitude/
https://aerotoolbox.com/atmcalc/
https://www.google.com/search?sxsrf=APq-WBtZHWHDjpIzFDZMkjL56h6EUsY7BA:1649071171797&q=Air+pressure+at+sea+level&sa=X&ved=2ahUKEwiQ2J3rpPr2AhWByosBHcInAf0Q1QJ6BAgZEAE
https://www.google.com/search?q=d%C4%91%E1%BB%99+cao+h%E1%BB%93+ch%C3%AD+minh+so+vs+m%E1%BB%B1c+n%C6%B0%E1%BB%9Bc+bi%E1%BB%83n&oq=d%C4%91%E1%BB%99+cao+h%E1%BB%93+ch%C3%AD+minh+so+vs+m%E1%BB%B1c+n%C6%B0%E1%BB%9Bc+bi%E1%BB%83n&aqs=chrome..69i57j33i10i160.10836j0j7&sourceid=chrome&ie=UTF-8
https://hoctin.vn/tinh-can-bac-2-trong-excel/
https://www.google.com/search?q=hpa+to+pa&oq=hpa+to+pa&aqs=chrome.0.0i19j0i10i19j0i10i19i22i30j0i19i22i30l7.1847j0j7&sourceid=chrome&ie=UTF-8
https://convertlive.com/vi/u/chuy%E1%BB%83n-%C4%91%E1%BB%95i/kilopascal/%C4%91%E1%BB%83/hectopascals#101.325
https://www.google.com/search?q=kPa+to+hpa&oq=kPa+to+hpa&aqs=chrome..69i57j0i19l7j0i19i22i30l2.4234j0j7&sourceid=chrome&ie=UTF-8
https://hoidap247.com/cau-hoi/1537336
https://vi.wikipedia.org/wiki/%C3%81p_su%E1%BA%A5t_kh%C3%AD_quy%E1%BB%83n
https://www.google.com/search?q=%C3%A1p+su%E1%BA%A5t+kh%C3%AD+quy%E1%BB%83n+t%E1%BA%A1i+m%E1%BB%B1c+n%C6%B0%E1%BB%9Bc+bi%E1%BB%83n&oq=a%C3%A1p+su%E1%BA%A5t+%E1%BB%9F+m%E1%BB%B1c+n%C6%B0%E1%BB%9B&aqs=chrome.1.69i57j0i22i30.5822j0j7&sourceid=chrome&ie=UTF-8
https://www.google.com/search?q=nhi%E1%BB%87t+%C4%91%E1%BB%99+h%C3%B4%E1%BB%93+ch%C3%AD+minh&oq=nhi%E1%BB%87t+%C4%91%E1%BB%99+h%C3%B4%E1%BB%93+ch%C3%AD+minh&aqs=chrome..69i57j0i10l4j0i22i30l3.3499j1j7&sourceid=chrome&ie=UTF-8
https://www.google.com/search?q=d%C4%91%E1%BB%99+cao+h%E1%BB%93+ch%C3%AD+minh+so+vs+m%E1%BB%B1c+n%C6%B0%E1%BB%9Bc+bi%E1%BB%83n&oq=d%C4%91%E1%BB%99+cao+h%E1%BB%93+ch%C3%AD+minh+so+vs+m%E1%BB%B1c+n%C6%B0%E1%BB%9Bc+bi%E1%BB%83n&aqs=chrome..69i57j33i10i160.10836j0j7&sourceid=chrome&ie=UTF-8
https://keisan.casio.com/exec/system/1224562962
https://www.google.com/search?sxsrf=APq-WBtZHWHDjpIzFDZMkjL56h6EUsY7BA:1649071171797&q=Air+pressure+at+sea+level&sa=X&ved=2ahUKEwiQ2J3rpPr2AhWByosBHcInAf0Q1QJ6BAgZEAE
https://www.mide.com/air-pressure-at-altitude-calculator
https://math24.net/barometric-formula.html
https://sciencing.com/calculate-tons-cooling-cooling-tower-10058467.html
https://www.google.com/search?q=calculate+temperature+from+barometer&oq=calculate+tem&aqs=chrome.1.69i57j35i39j0i19l4j69i60l2.5344j0j7&sourceid=chrome&ie=UTF-8
https://www.google.com.vn/search?q=in+hg+to+mmhg&ie=UTF-8&oe=UTF-8&hl=en-vn&client=safari
https://www.quora.com/How-can-I-calculate-the-temperature-at-altitude
https://en.wikipedia.org/wiki/Atmospheric_temperature#Temperature_versus_height
https://www.quora.com/How-can-I-calculate-the-temperature-at-altitude
https://lytuong.net/quy-doi-tu-do-f-sang-do-c/
https://treelinebackpacker.com/2013/05/06/calculate-temperature-change-with-elevation/#:~:text=You%20will%20lose%20an%20average,metric%20conversion%20in%20the%20comments).
https://www.google.com/search?q=calculate+temperature+from+altitude&oq=temperature+from+alt&aqs=chrome.2.69i57j0i22i30l9.15386j0j7&sourceid=chrome&ie=UTF-8
https://www.omnicalculator.com/physics/altitude-temperature
https://climateknowledgeportal.worldbank.org/country/vietnam/impacts-sea-level-rise
https://www.google.com/search?q=temperature+at+sea+level+vietnam&sxsrf=APq-WBuGYZR703GKUtSiBoDVbz0JBgdkYw%3A1649092809062&ei=yShLYvi4A9qDoASzv4i4AQ&ved=0ahUKEwi4wde49fr2AhXaAYgKHbMfAhcQ4dUDCA4&uact=5&oq=temperature+at+sea+level+vietnam&gs_lcp=Cgdnd3Mtd2l6EAMyBQghEKABOgcIIxCwAxAnOgcIABBHELADOgQIIxAnOgYIABAWEB46CAghEBYQHRAeOgQIIRAVOgUIABDLAToHCCEQChCgAUoECEEYAEoECEYYAFC3A1i0MWCNM2gKcAF4AoABrwOIAYwYkgEKMC4xMi4yLjEuMZgBAKABAcgBBcABAQ&sclient=gws-wiz
https://weatherspark.com/y/68301/Average-Weather-in-Hamburg-Germany-Year-Round
https://seatemperature.info/hamburg-water-temperature.html
https://www.google.com/search?q=temperature+at+sea+level+hamburg&oq=temperature+at+sea+level+ham&aqs=chrome.1.69i57j33i160l3.9282j0j4&sourceid=chrome&ie=UTF-8

https://www.convertunits.com/from/mmHg/to/InHg
https://www.convertunits.com/from/mmHg/to/inH2O#:~:text=How%20many%20mmHg%20in%201,The%20answer%20is%201.8683201548767.
https://www.convertunits.com/from/pascal/to/inHg
https://www.convertunits.com/from/pa/to/mmHg
https://www.nature.com/articles/s41598-019-49703-y
https://amerisleep.com/blog/heart-rate-variability-and-sleep/
https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4263095/
https://github.com/MNVolkov/navi_demo/blob/master/main.c
https://www.whoop.com/thelocker/heart-rate-variability-hrv/
https://myamazfit.ru/threads/bip-prilozhenija-dlja-bipos-elf.1174/page-70#post-49439
https://www.thecoolist.com/how-to-use-a-barometer/
https://www.almanac.com/content/predicting-weather-using-barometer-and-wind-direction
https://sciencing.com/predict-weather-barometer-5767204.html
https://education.seattlepi.com/ways-predict-storm-air-pressure-6349.html
https://www.google.com.vn/search?q=predict+rain+barometric+pressure&ie=UTF-8&oe=UTF-8&hl=en-vn&client=safari
https://imotions.com/blog/heart-rate-variability/
https://www.google.com.vn/search?q=how+to+calculate+hrv+from+heart+rate&ie=UTF-8&oe=UTF-8&hl=en-vn&client=safari
https://kienthuckhoahoc.org/suc-khoe-y-hoc/suc-khoe/bien-thien-tan-so-tim-hrv-va-y-nghia-cua-thong-so-nay-la-gi-a8zu
https://vnreview.vn/thread-old/bien-thien-tan-so-tim-hrv-va-y-nghia-cua-thong-so-nay-la-gi.3342256
https://www.garmin.com/vi-VN/blog/1-phuong-thuc-khac-de-nhan-biet-su-met-moi-su-bien-doi-cua-nhip-tim-hrv/
https://support.garmin.com/vi-VN/?faq=lWqSVlq3w76z5WoihLy5f8
https://support.garmin.com/vi-VN/?faq=04pnPSBTYSAYL9FylZoUl5
https://journals.sagepub.com/doi/pdf/10.1177/1460458219827349
https://www.google.com.vn/search?q=heart+rate+variability+là+gì&ie=UTF-8&oe=UTF-8&hl=en-vn&client=safari
http://www.scielo.edu.uy/pdf/cp/v13n2/en_1688-4221-cp-13-02-275.pdf
https://help.fitbit.com/articles/en_US/Help_article/1314.htm
https://blog.fitbit.com/heart-rate-during-sleep-stages/
https://www.cnet.com/culture/entertainment/spider-man-no-way-home-ending-explained-and-all-your-questions-answered-sequels/
https://www.nature.com/articles/1300146
https://www.google.com.vn/search?q=determine+deep+and+light+sleep+using+heart+rate&ie=UTF-8&oe=UTF-8&hl=en-vn&client=safari
https://education.seattlepi.com/ways-predict-storm-air-pressure-6349.html
https://www.google.com.vn/search?q=predict+rain+barometric+pressure&ie=UTF-8&oe=UTF-8&hl=en-vn&client=safari
https://github.com/MNVolkov/libbip/blob/master/libbip.h
https://myamazfit.ru/threads/bip-razrabotka-prilozhenij-dlja-bipos-sdk.1170/
https://academic.oup.com/annweh/article/64/4/350/5735350?login=false
https://www.google.com.vn/search?q=how+can+actigraph+determine+sleep+and+wake&ie=UTF-8&oe=UTF-8&hl=en-vn&client=safari
https://www.google.com.vn/search?client=safari&hl=en-GB&sxsrf=APq-WBsEsaUV9QqkJfkOg9fTWt-Z82XDkA:1648387930852&q=Actigraphy+sleep+study&sa=X&ved=2ahUKEwih6sfIs-b2AhWE-2EKHXWPAu8Q1QJ6BAg6EAE&biw=1366&bih=985&dpr=2
https://www.nosleeplessnights.com/how-do-sleep-trackers-work/
https://www.quora.com/How-do-fitness-bands-track-my-sleep-How-do-they-sense-the-light-sleep-and-deep-sleep
https://theconversation.com/are-sleep-trackers-accurate-heres-what-researchers-currently-know-152500
https://www.google.com.vn/search?q=how+can+smartwatch+measure+sleep&ie=UTF-8&oe=UTF-8&hl=en-vn&client=safari
https://daynhauhoc.com/t/thuat-toan-doi-kieu-chuoi-char-sang-kieu-so-int-trong-c/16549
https://www.google.com/search?q=chuy%E1%BB%83n+char+a+sang+char+b&oq=chuy%E1%BB%83n+char+a+sang+char+b&aqs=chrome..69i57j33i22i29i30j33i160.6411j0j7&sourceid=chrome&ie=UTF-8
https://aicurious.io/posts/2016-07-28-toan-tu-trong-c/
https://www.google.com/search?q=ho%E1%BA%B7c+trong+c&oq=ho%E1%BA%B7c+trong+c&aqs=chrome.0.69i59j0i512l2j0i22i30l4.6957j0j7&sourceid=chrome&ie=UTF-8
https://www.convertunits.com/from/inH2O/to/mmHg
https://www.google.com/search?q=100+in+h20+to+mmhg&sxsrf=APq-WBtdUqLKmNmxeV1qXZj8dIXV67Et8w%3A1648658799273&ei=b4lEYqWBEI3K2roP8I246Ag&ved=0ahUKEwjl9NfQpO72AhUNpVYBHfAGDo0Q4dUDCA4&uact=5&oq=100+in+h20+to+mmhg&gs_lcp=Cgdnd3Mtd2l6EAM6BwgAEEcQsAM6BAgjECc6BQgAEMsBOgYIABAWEB46CAgAEBYQChAeOggIIRAWEB0QHkoECEEYAEoECEYYAFDLBFjjDGD4DmgBcAF4AIAB3wGIAbAHkgEFMC41LjGYAQCgAQHIAQjAAQE&sclient=gws-wiz
https://www.convertunits.com/from/in+H2O/to/pascal
https://www.google.com/search?q=100+in+h20+to+pa&sxsrf=APq-WBsvzsRHupSEj5SK3kIycDeo2OxOqA%3A1648658678282&ei=9ohEYtDcEKSz2roP-o-O6AE&ved=0ahUKEwjQr_-WpO72AhWkmVYBHfqHAx0Q4dUDCA4&uact=5&oq=100+in+h20+to+pa&gs_lcp=Cgdnd3Mtd2l6EAMyBQgAEMsBOgcIABBHELADOgcILhCwAxBDOgcIABCwAxBDOgUIABCABDoECAAQQzoGCAAQFhAeOggIABAWEAoQHjoCCCZKBAhBGABKBAhGGABQpwxYuk5gwk9oBXABeACAAasBiAHMCpIBBDAuMTCYAQCgAQHIAQrAAQE&sclient=gws-wiz
https://huphaco.vn/don-vi-mpa-la-gi/
https://www.google.com/search?q=100+feet+to+meters&sxsrf=APq-WBss71QNPLUpdTprf82vid_uUf6Lhg%3A1648657327159&ei=r4NEYp6RCYKz2roPz6C98As&oq=100+feet+to&gs_lcp=Cgdnd3Mtd2l6EAMYADIFCAAQywEyBQgAEMsBMgUIABDLATIFCAAQywEyBQgAEMsBMgUIABDLATIFCAAQywEyBQgAEMsBMgUIABDLATIFCAAQywE6BggjECcQEzoECCMQJzoECAAQQzoLCAAQgAQQsQMQgwE6BggAEAoQQzoICAAQgAQQsQM6CAgAELEDEIMBOgUIABCABDoLCC4QgAQQsQMQ1AI6BQguELEDOgoIABCABBBGEIICOggILhCABBCxAzoFCC4QgAQ6BAgAEA06BAguEA06BggAEA0QHjoJCAAQyQMQDRAeOggIABAWEAoQHjoFCC4QywFKBAhBGABKBAhGGABQAFiFGGCIIGgBcAF4AIABqwGIAbgNkgEEMC4xMpgBAKABAcABAQ&sclient=gws-wiz
https://www.nationalgeographic.org/encyclopedia/barometer/
https://cboard.cprogramming.com/c-programming/99815-how-convert-decimal-floating-point-number.html
https://www.google.com/search?q=decimal+to+float+c&oq=decimal+to+float+c&aqs=chrome..69i57j0i512j0i22i30l8.7568j0j7&sourceid=chrome&ie=UTF-8
https://stackoverflow.com/questions/1042099/how-do-i-convert-int-decimal-to-float-in-c
https://www.google.com/search?q=1pa+to+mmhg&sxsrf=APq-WBsl7OyvPTYNm5vLlpqD5g9rg2mHvQ%3A1648657224293&ei=SINEYtuoEaLL2roP8qOw0Ao&ved=0ahUKEwjb7Nbhnu72AhWipVYBHfIRDKoQ4dUDCA4&uact=5&oq=1pa+to+mmhg&gs_lcp=Cgdnd3Mtd2l6EAMyBQgAEMsBMggIABAWEAoQHjoHCAAQRxCwAzoECAAQEzoKCAAQCBANEB4QEzoMCAAQCBANEAoQHhATOgYIABAWEB46BwgAEAoQywFKBAhBGABKBAhGGABQvgRYhhdgoBloAXABeACAAdUBiAHOCpIBBTAuNi4ymAEAoAEByAEIwAEB&sclient=gws-wiz
https://translate.google.com/?sl=ru&tl=vi&text=%2F%2F%09%D0%BA%D0%BE%D1%8D%D1%84%D1%84%D0%B8%D1%86%D0%B8%D0%B5%D0%BD%D1%82%D1%8B%20%D0%BF%D1%80%D0%B8%D0%B2%D0%B5%D0%B4%D0%B5%D0%BD%D0%B8%D1%8F%20%D0%B8%D0%B7%20%D0%9F%D0%B0%D1%81%D0%BA%D0%B0%D0%BB%D0%B5%D0%B9&op=translate
https://www.google.com/search?q=1hpa+to+pa&oq=1+hpa+&aqs=chrome.2.69i57j0i512j0i10i512j0i512j0i10i22i30j0i22i30l5.4617j0j7&sourceid=chrome&ie=UTF-8
https://free.nchc.org.tw/osdn//sfnet/h/hl/hlanguage/Source/include/math.h
https://www.tutorialspoint.com/c_standard_library/math_h.htm
https://phptravels.vn/thu-vien-toan-hoc-math-h-trong-c/
https://vietjack.com/thu-vien-c/math-h-trong-c.jsp
https://nguyenvanhieu.vn/thu-vien-math-h-trong-c/
https://pubs.opengroup.org/onlinepubs/9699919799/
https://pubs.opengroup.org/onlinepubs/007904875/basedefs/math.h.html
https://stackoverflow.com/questions/24624039/how-to-get-hrv-heart-rate-variability-from-hr-heart-rate
https://www.google.com/search?q=calculate+hrv+from+bpm+imotion&sxsrf=APq-WBv8dWQIpdFi0F0A_TvuSvz59Dr8QA%3A1648566141738&ei=fR9DYvDcLI-wmAWemZG4DQ&ved=0ahUKEwiwj5G6y-v2AhUPGKYKHZ5MBNcQ4dUDCA4&uact=5&oq=calculate+hrv+from+bpm+imotion&gs_lcp=Cgdnd3Mtd2l6EAMyBQghEKABOgcIABBHELADOgYIABAWEB46CAghEBYQHRAeOgQIIRAVSgQIQRgASgQIRhgAUOcBWKEYYIcaaAFwAXgAgAG9AYgBkwmSAQMwLjiYAQCgAQHIAQjAAQE&sclient=gws-wiz
https://myamazfit.ru/threads/bip-prilozhenija-dlja-bipos-elf.1174/page-117#post-56016
https://myamazfit.ru/threads/bip-prilozhenija-dlja-bipos-elf.1174/page-70#post-49439
https://myamazfit.ru/threads/bip-prilozhenija-dlja-bipos-elf.1174/page-70#lg=post-49439&slide=0
https://myamazfit.ru/threads/bip-prilozhenija-dlja-bipos-elf.1174/page-70#lg=post-49439&slide=1
https://myamazfit.ru/threads/bip-prilozhenija-dlja-bipos-elf.1174/page-70#lg=post-49439&slide=2
https://sciencing.com/build-weather-instruments-kids-4779859.html
https://sciencing.com/recalibrate-springfield-barometer-7672789.html
https://sciencing.com/schatz-barometer-instructions-8099219.html
https://sciencing.com/directions-making-barometer-5372129.html
https://sciencing.com/2-types-barometers-8524023.html
https://sciencing.com/glass-barometer-filling-instructions-7596463.html
https://sciencing.com/easy-homemade-weather-instruments-kids-7974126.html
https://sciencing.com/high-low-reading-barometric-pressure-5814364.html
https://sciencing.com/set-read-barometer-18498.html
https://sciencing.com/instruments-used-measuring-air-pressure-7158667.html
https://sciencing.com/read-weather-swan-barometer-6320226.html
https://sciencing.com/understand-barometric-pressure-readings-5397464.html
https://sciencing.com/three-different-types-clouds-6967664.html
https://sciencing.com/make-simple-weather-station-4867576.html
https://www.google.com/search?q=predict+weather+with+barometer&oq=predict+weather+with+ba&aqs=chrome.0.0i19j69i57.10907j0j7&sourceid=chrome&ie=UTF-8
https://www.artofmanliness.com/lifestyle/gear/fair-or-foul-how-to-use-a-barometer/
https://www.almanac.com/content/predicting-weather-using-barometer-and-wind-direction
https://www.google.com/search?q=d%C4%91%C6%A1n+v%E1%BB%8B+pa+l%C3%A0+g%C3%AC&oq=d%C4%91%C6%A1n+v%E1%BB%8B+pa+l%C3%A0+g%C3%AC&aqs=chrome..69i57j0i22i30l2j0i10i22i30l2j0i22i30l3.9610j0j7&sourceid=chrome&ie=UTF-8
https://huphaco.vn/don-vi-mpa-la-gi/
https://osdn.net/projects/sfnet_hlanguage/downloads/Source/include/math.h/
https://sites.uclouvain.be/SystInfo/usr/include/math.h.html
https://www.google.com/search?q=math.h+download+c&sxsrf=APq-WBv65yDnnVrRdIHElbx4SMmrndc9Yw%3A1648623374665&ei=Dv9DYq-UKNmB1e8PnIqSsAY&oq=math.h+download+&gs_lcp=Cgdnd3Mtd2l6EAMYAjIGCAAQFhAeMgYIABAWEB4yBggAEBYQHjIGCAAQFhAeMgYIABAWEB4yBggAEBYQHjIGCAAQFhAeMgYIABAWEB4yBggAEBYQHjIGCAAQFhAeOgcIABBHELADSgQIQRgASgQIRhgAUMgBWMgBYO8UaAFwAXgAgAHOAYgBzgGSAQMyLTGYAQCgAQHIAQjAAQE&sclient=gws-wiz
https://gist.github.com/vertrigo/4033545
https://www.google.com/search?q=math.h+download&oq=math.h+do&aqs=chrome.1.0i19l2j69i57j0i19j0i19i22i30l6.83737j0j7&sourceid=chrome&ie=UTF-8
https://time.is/
https://sciencing.com/predict-weather-barometer-5767204.html#:~:text=The%20basic%20rules%20of%20thumb,more%20the%20weather%20will%20change.
https://sciencing.com/predict-weather-barometer-5767204.html
https://www.nationalgeographic.org/encyclopedia/barometer/
https://time.is/Germany
https://time.is/London
https://github.com/MNVolkov/libbip
https://v1study.com/c-ham-toan-hoc-math-a75.html
https://www.medicinenet.com/what_is_heart_rate_variability_hrv/article.htm
https://www.google.com/search?q=b%C3%ACnh+ph%C6%B0%C6%A1ng+trong+c%2B%2B&oq=b%C3%ACnh+ph%C6%B0%C6%A1ng+trong+c&aqs=chrome.0.0i512j69i57j0i512l2j0i22i30l6.6278j0j7&sourceid=chrome&ie=UTF-8
https://viettuts.vn/lap-trinh-cpp/ham-math-trong-cpp
https://imotions.com/blog/heart-rate-variability/
https://www.google.com/search?q=calculate+hrv+from+bpm+imotion&sxsrf=APq-WBv8dWQIpdFi0F0A_TvuSvz59Dr8QA%3A1648566141738&ei=fR9DYvDcLI-wmAWemZG4DQ&ved=0ahUKEwiwj5G6y-v2AhUPGKYKHZ5MBNcQ4dUDCA4&uact=5&oq=calculate+hrv+from+bpm+imotion&gs_lcp=Cgdnd3Mtd2l6EAMyBQghEKABOgcIABBHELADOgYIABAWEB46CAghEBYQHRAeOgQIIRAVSgQIQRgASgQIRhgAUOcBWKEYYIcaaAFwAXgAgAG9AYgBkwmSAQMwLjiYAQCgAQHIAQjAAQE&sclient=gws-wiz
https://stackoverflow.com/questions/24624039/how-to-get-hrv-heart-rate-variability-from-hr-heart-rate
https://www.google.com/search?q=calculate+hrv+from+bpm&sxsrf=APq-WBvSJwxweIUQKyRNn-LBQ5NIkM99ew%3A1648566136065&ei=eB9DYonUA7KwmAWQ95Yg&oq=calculate+hrv+fr&gs_lcp=Cgdnd3Mtd2l6EAMYADIFCAAQywEyBQgAEMsBMgUIABDLATIFCAAQywEyBggAEBYQHjIGCAAQFhAeMgYIABAWEB4yBggAEBYQHjoHCAAQRxCwAzoGCAAQDRAeSgQIQRgASgQIRhgAUP0EWMQIYO8QaAFwAXgAgAHiAYgB4AOSAQUwLjIuMZgBAKABAcgBCMABAQ&sclient=gws-wiz
https://www.google.com/search?q=calculate+hrv&sxsrf=APq-WBvzDpiSYEliIsyCQlUx1lBj-2Ra4A%3A1648565954321&ei=wh5DYp2dE7rFmAXQ9ZzQBg&ved=0ahUKEwidhuLgyuv2AhW6IqYKHdA6B2oQ4dUDCA4&uact=5&oq=calculate+hrv&gs_lcp=Cgdnd3Mtd2l6EAMyBQgAEMsBMgUIABDLATIFCAAQywEyBQgAEMsBMgUIABDLATIFCAAQywEyBggAEA0QHjIGCAAQFhAeMgYIABAWEB4yBggAEBYQHjoECCMQJzoECAAQAzoECAAQQzoICAAQgAQQsQM6CwgAEIAEELEDEIMBOhEILhCABBCxAxCDARDHARDRAzoGCCMQJxATOgUIABCABDoECC4QQzoICC4QgAQQsQM6BwgAELEDEEM6CggAELEDEIMBEAo6BwgAEAoQywFKBAhBGABKBAhGGABQAFjqEWCAF2gAcAF4AIAB4AGIAfMQkgEFMC45LjSYAQCgAQHAAQE&sclient=gws-wiz
https://www.cnet.com/health/sleep/how-sleep-cycles-work-rem-vs-deep-sleep/
https://www.google.com/search?q=hrv+change+in+light+and+deep+sleep&sxsrf=APq-WBsRHOTH0w6pzl5JkUlPmjetFwUHDQ%3A1648565937631&ei=sR5DYtOEJq6Rr7wPgf2S0Ak&ved=0ahUKEwiToefYyuv2AhWuyIsBHYG-BJoQ4dUDCA4&uact=5&oq=hrv+change+in+light+and+deep+sleep&gs_lcp=Cgdnd3Mtd2l6EAM6BwgAEEcQsANKBAhBGABKBAhGGABQgwZYmApglgxoAXABeACAAYACiAGHBpIBBTAuMy4xmAEAoAEByAEIwAEB&sclient=gws-wiz
https://www.google.com/search?q=hvr+change+in+light+and+deep+sleep&oq=hvr+change+in+light+and+deep+sleep&aqs=chrome..69i57j33i160.7266j0j7&sourceid=chrome&ie=UTF-8
https://www.google.com/search?q=predict+weather+with+barometer&oq=predict+weather+with+ba&aqs=chrome.0.0i19j69i57.10907j0j7&sourceid=chrome&ie=UTF-8&sxsrf=APq-WBtzeNCgp66Fw48X3M9rLwHB57RmkA:1648913943247&sec_act=sr


*/

void thegioi1(){  // Lấy giờ Úc (Giờ VN +3) , Đức (Giờ VN -6) , Los Angeles (Giờ VN -15) , London (Giờ VN -7). Ký tự nơi ở sẽ thấy kí tự theo Mã chung ISO và cách 15 px, Giờ + thì > 24, Giờ - thì < 0)
  // Vì lý dó có các nước như Đức, London là có xài thêm giờ mùa đông nên viết lại code như sau
  //Các nước có giờ mùa đông
//-	Đức
//-	Anh (London)
//-	Đức và anh xài chung 1 giờ mùa đông
//Bắt đầu: Ngày chủ nhật cuối cùng của tháng 3 (31- 6= 25 =< x =< 31) lúc 1h sáng: Tăng thêm 1h --> Giờ VN -5 (Cho Đức), Giờ VN -6 (Cho London)
//Kết thúc: Ngày chủ nhật cuối cùng của tháng 10 (31- 6= 25 =< x =< 31) lúc 2h sáng
//-	Los Angeles
//-	Xài giờ riêng
//-	Bắt đầu: Ngày chủ nhật thứ 2 của tháng 3 (8 =< x =< 8+6) lúc 2h sáng: Tăng thêm 1h --> Giờ VN -14
//-	Kết thúc: Ngày chủ nhật thứ 1 của tháng  11 (1 =< x =< 7) lúc 2h
// Hàm gán giá trị, phải khai báo trước 

//Dựa vào hàm này (Trong app Music control): _sprintf(data, "%02s.%02d.%02d",weekday_string[dt.weekday], dt.day, dt.month);
//Lưu ý nếu gọi hàm Thứ thì trong kiểu dữ liệu phải là %02s (String là chữ)
//char *weekday_string_en[] = {"??", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
//Ví dụ cách hiển thị Thứ:
//char *weekday_string[] = {"??", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
//text_out(weekday_string[dt.weekday],155,100);	// печатаем день недели
//Ỏ đây có thể thấy các thứ sẽ được sắp xếp như trên là: ???. Thứ 2, Thứ 3, Thứ 4, Thứ 5, Thứ 6, Thứ 7, Chủ nhật sẽ tương ứng lần lượt với dt.weekday = 1, 2, 3, 4, 5, 6, 7, 8
//Các dữ liệu thời gian có thể lấy là: Giây (dt.sec, %02d), phút (dt.min, %02d), giờ (dt.hour, %02d), ngày (dt.day, %02d), tháng (dt.month, %02d), thứ (dt.weekday, %02s), năm (dt.year, %02d)

  set_bg_color(COLOR_WHITE);
  fill_screen_bg();
  set_fg_color(COLOR_BLACK);
text_out_center("WORLD CLOCK", 88, 6);
text_out_center("TONY", 88, 30);

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 54, 172, 85);
  set_bg_color(COLOR_BLACK);
  text_out_center("VNM: ", 60, 58);
  char clock_time_VN[8]; 			//	текст время		12:34
		struct datetime_ dt;
		get_current_date_time(&dt);
		_sprintf(clock_time_VN, "%02d:%02d", dt.hour, dt.min);
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_VN, 80 , 58, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);     


  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 90, 86, 133);
  set_bg_color(COLOR_BLACK);
  text_out_center("GER: ", 23, 104);
  int GE3;
  GE3 = 31;
   int GE10;
  GE10 = 31;
  char clock_time_GE[8]; 			//	текст время		12:34
           //Có phải trong tháng 3 đến tháng 10 không? Không thì sao: Đã làm
			 if ( ( dt.month >= 3) && ( dt.month <= 10) ) {
				 //Xét tháng 3 trước
                  if ( dt.month == 3){
					  //Xét trong tuần cuối cùng? Còn lại thì sao: Đã làm
					  if ( ( dt.day <= 31) && ( dt.day >= 25) ){
 	                     if ( dt.weekday == 8){
							 GE3 = dt.day;
					//Xét riêng trong ngày chủ nhật lúc 1h sáng mới thay đổi đồng hồ
								if ( dt.hour - 6 >= 1){ // Giờ ở Đức từ 1h sáng chủ nhật là cộng thêm 1h
									_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 1, dt.min); 
								}
					//Còn lại là không bị thay đổi +1h
								else if (dt.hour - 6 < 0) {
									_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 24, dt.min);
												}
								else {
									_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6, dt.min);
									}
									}
					//Xét các ngày sau ngày chủ nhật trong tuần cuối đó (Bị thay đổi giống ngày chủ nhật: +1h)
						else if ( dt.day > GE3) {
							if (dt.hour - 6 + 1 < 0) {
								 _sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 24 + 1, dt.min);
								}
							else {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 1, dt.min);
								}
								}
					//Còn lại không thuộc 2 trường hợp trên là (Trước ngày chủ nhật)
						else if (dt.hour - 6 < 0) {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 24, dt.min);
							}
							else {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6, dt.min);
								}
								} //Hết xét tuần cuối cùng
						else if (dt.hour - 6 < 0) {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 24, dt.min);
								}
							else {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6, dt.min);
								}
					} // Dấu kết cho tháng 3
				//Xét đến tháng 10
                  if ( dt.month == 10){
					  //Xét trong tuần cuối cùng? Còn lại thì sao: Đã làm
					  if ( ( dt.day <= 31) && ( dt.day >= 25) ){
 	                     if ( dt.weekday == 8){
							 GE10 = dt.day;
					//Xét riêng trong ngày chủ nhật lúc 2h sáng mới thay đổi đồng hồ
								if ( dt.hour - 6 + 1 >= 2){ // Giờ ở Đức từ 2h sáng chủ nhật là về lại bình thường
									_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6, dt.min); 
								}
					//Còn lại là không bị thay đổi +1h
								else if (dt.hour - 6 + 1 < 0) {
									_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 24 + 1, dt.min);
												}
								else {
									_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 1, dt.min);
									}
									}
					//Xét các ngày sau ngày chủ nhật trong tuần cuối đó (Bị thay đổi giống ngày chủ nhật: +1h)
						else if ( dt.day > GE10) {
							if (dt.hour - 6 < 0) {
								 _sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 24, dt.min);
								}
							else {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6, dt.min);
								}
								}
					//Còn lại không thuộc 2 trường hợp trên là (Trước ngày chủ nhật)
						else if (dt.hour - 6 + 1 < 0) {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 24 + 1, dt.min);
							}
							else {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 1, dt.min);
								}
								} //Hết xét tuần cuối cùng
						else if (dt.hour - 6 + 1 < 0) {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 24 + 1, dt.min);
								}
							else {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 1, dt.min);
								}
					} // Dấu kết cho tháng 10														  
				//Xét các tháng còn lại trong khoảng từ tháng 3 đến tháng 10
					else if (dt.hour - 6 + 1 < 0) {
							_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 24 + 1, dt.min);
						}
						else {
							_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 1, dt.min);
						}
			} //Hết cho tháng 3 và tháng 10
			else if (dt.hour - 6 < 0) {
					_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6 + 24, dt.min);
					}
				else {
					_sprintf(clock_time_GE, "%02d:%02d", dt.hour - 6, dt.min);
					}
			 
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_GE, 38 , 104, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);     

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(90, 90, 172, 133);
  set_bg_color(COLOR_BLACK);
  text_out_center("AUS: ", 110, 104);
    char clock_time_AU[8]; 			//	текст время		12:34
		
		 if (dt.hour + 3 > 24) {
  _sprintf(clock_time_AU, "%02d:%02d", dt.hour + 3 - 24, dt.min);
 }
  else {
 _sprintf(clock_time_AU, "%02d:%02d", dt.hour + 3, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_AU, 125 , 104, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);    
  
    set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 140, 86, 169);
  set_bg_color(COLOR_BLACK);
  text_out_center("USA: ", 23, 147);
     int US3;
  US3 = 31;
   int US10;
  US10 = 31;
  char clock_time_US[8]; 			//	текст время		12:34
           //Có phải trong tháng 3 đến tháng 11 không? Không thì sao: Đã làm
		   //-	Bắt đầu: Ngày chủ nhật thứ 2 của tháng 3 (8 =< x =< 8+6) lúc 2h sáng: Tăng thêm 1h --> Giờ VN -14
			//-	Kết thúc: Ngày chủ nhật thứ 1 của tháng  11 (1 =< x =< 7) lúc 2h
			 if ( ( dt.month >= 3) && ( dt.month <= 11) ) {
				 //Xét tháng 3 trước
                  if ( dt.month == 3){
					  //Xét trong tuần cuối cùng? Còn lại thì sao: Đã làm
					  if ( ( dt.day <= 14) && ( dt.day >= 8) ){
 	                     if ( dt.weekday == 8){
							 US3 = dt.day;
					//Xét riêng trong ngày chủ nhật lúc 1h sáng mới thay đổi đồng hồ
								if ( dt.hour - 15 >= 2){ // Giờ ở USA từ 2h sáng chủ nhật là cộng thêm 1h
									_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 1, dt.min); 
								}
					//Còn lại là không bị thay đổi +1h
								else if (dt.hour - 15 < 0) {
									_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 24, dt.min);
												}
								else {
									_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15, dt.min);
									}
									}
					//Xét các ngày sau ngày chủ nhật trong tuần cuối đó (Bị thay đổi giống ngày chủ nhật: +1h)
						else if ( dt.day > US3) {
							if (dt.hour - 15 + 1 < 0) {
								 _sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 24 + 1, dt.min);
								}
							else {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 1, dt.min);
								}
								}
					//Còn lại không thuộc 2 trường hợp trên là (Trước ngày chủ nhật)
						else if (dt.hour - 15 < 0) {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 24, dt.min);
							}
							else {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15, dt.min);
								}
								} //Hết xét tuần cuối cùng
						else if (dt.hour - 15 < 0) {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 24, dt.min);
								}
							else {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15, dt.min);
								}
					} // Dấu kết cho tháng 3
				//Xét đến tháng 11
                  if ( dt.month == 11){
					  //Xét trong tuần cuối cùng? Còn lại thì sao: Đã làm
					  if ( ( dt.day <= 7) && ( dt.day >= 1) ){
 	                     if ( dt.weekday == 8){
							 US10 = dt.day;
					//Xét riêng trong ngày chủ nhật lúc 2h sáng mới thay đổi đồng hồ
								if ( dt.hour - 15 + 1 >= 2){ // Giờ ở Đức từ 2h sáng chủ nhật là về lại bình thường
									_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15, dt.min); 
								}
					//Còn lại là không bị thay đổi +1h
								else if (dt.hour - 15 + 1 < 0) {
									_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 24 + 1, dt.min);
												}
								else {
									_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 1, dt.min);
									}
									}
					//Xét các ngày sau ngày chủ nhật trong tuần cuối đó (Bị thay đổi giống ngày chủ nhật: +1h)
						else if ( dt.day > US10) {
							if (dt.hour - 15 < 0) {
								 _sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 24, dt.min);
								}
							else {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15, dt.min);
								}
								}
					//Còn lại không thuộc 2 trường hợp trên là (Trước ngày chủ nhật)
						else if (dt.hour - 15 + 1 < 0) {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 24 + 1, dt.min);
							}
							else {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 1, dt.min);
								}
								} //Hết xét tuần cuối cùng
						else if (dt.hour - 15 + 1 < 0) {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 24 + 1, dt.min);
								}
							else {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 1, dt.min);
								}
					} // Dấu kết cho tháng 10														  
				//Xét các tháng còn lại trong khoảng từ tháng 3 đến tháng 10
					else if (dt.hour - 15 + 1 < 0) {
							_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 24 + 1, dt.min);
						}
						else {
							_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 1, dt.min);
						}
			} //Hết cho tháng 3 và tháng 10
			else if (dt.hour - 15 < 0) {
					_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15 + 24, dt.min);
					}
				else {
					_sprintf(clock_time_US, "%02d:%02d", dt.hour - 15, dt.min);
					}   
	  
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_US, 38 , 147, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176); 

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(90, 140, 172, 169);
  set_bg_color(COLOR_BLACK);
  text_out_center("GBR: ", 110, 147); //Nước anh
        int GB3;
  GB3 = 31;
   int GB10;
  GB10 = 31;
  char clock_time_GB[8]; 			//	текст время		12:34
           //Có phải trong tháng 3 đến tháng 10 không? Không thì sao: Đã làm
			 if ( ( dt.month >= 3) && ( dt.month <= 10) ) {
				 //Xét tháng 3 trước
                  if ( dt.month == 3){
					  //Xét trong tuần cuối cùng? Còn lại thì sao: Đã làm
					  if ( ( dt.day <= 31) && ( dt.day >= 25) ){
 	                     if ( dt.weekday == 8){
							 GB3 = dt.day;
					//Xét riêng trong ngày chủ nhật lúc 1h sáng mới thay đổi đồng hồ
								if ( dt.hour - 7 >= 1){ // Giờ ở Đức từ 1h sáng chủ nhật là cộng thêm 1h
									_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 1, dt.min); 
								}
					//Còn lại là không bị thay đổi +1h
								else if (dt.hour - 7 < 0) {
									_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 24, dt.min);
												}
								else {
									_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7, dt.min);
									}
									}
					//Xét các ngày sau ngày chủ nhật trong tuần cuối đó (Bị thay đổi giống ngày chủ nhật: +1h)
						else if ( dt.day > GB3) {
							if (dt.hour - 7 + 1 < 0) {
								 _sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 24 + 1, dt.min);
								}
							else {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 1, dt.min);
								}
								}
					//Còn lại không thuộc 2 trường hợp trên là (Trước ngày chủ nhật)
						else if (dt.hour - 7 < 0) {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 24, dt.min);
							}
							else {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7, dt.min);
								}
								} //Hết xét tuần cuối cùng
						else if (dt.hour - 7 < 0) {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 24, dt.min);
								}
							else {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7, dt.min);
								}
					} // Dấu kết cho tháng 3
				//Xét đến tháng 10
                  if ( dt.month == 10){
					  //Xét trong tuần cuối cùng? Còn lại thì sao: Đã làm
					  if ( ( dt.day <= 31) && ( dt.day >= 25) ){
 	                     if ( dt.weekday == 8){
							 GB10 = dt.day;
					//Xét riêng trong ngày chủ nhật lúc 2h sáng mới thay đổi đồng hồ
								if ( dt.hour - 7 + 1 >= 2){ // Giờ ở Đức từ 2h sáng chủ nhật là về lại bình thường
									_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7, dt.min); 
								}
					//Còn lại là không bị thay đổi +1h
								else if (dt.hour - 7 + 1 < 0) {
									_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 24 + 1, dt.min);
												}
								else {
									_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 1, dt.min);
									}
									}
					//Xét các ngày sau ngày chủ nhật trong tuần cuối đó (Bị thay đổi giống ngày chủ nhật: +1h)
						else if ( dt.day > GB10) {
							if (dt.hour - 7 < 0) {
								 _sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 24, dt.min);
								}
							else {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7, dt.min);
								}
								}
					//Còn lại không thuộc 2 trường hợp trên là (Trước ngày chủ nhật)
						else if (dt.hour - 7 + 1 < 0) {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 24 + 1, dt.min);
							}
							else {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 1, dt.min);
								}
								} //Hết xét tuần cuối cùng
						else if (dt.hour - 7 + 1 < 0) {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 24 + 1, dt.min);
								}
							else {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 1, dt.min);
								}
					} // Dấu kết cho tháng 10														  
				//Xét các tháng còn lại trong khoảng từ tháng 3 đến tháng 10
					else if (dt.hour - 7 + 1 < 0) {
							_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 24 + 1, dt.min);
						}
						else {
							_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 1, dt.min);
						}
			} //Hết cho tháng 3 và tháng 10
			else if (dt.hour - 7 < 0) {
					_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7 + 24, dt.min);
					}
				else {
					_sprintf(clock_time_GB, "%02d:%02d", dt.hour - 7, dt.min);
					}


		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_GB, 125 , 147, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176); 


  repaint_screen_lines(0, 176);
 
}

void thegioi2(){  // Lấy giờ Nga (Giờ VN -4) , Trung  (Giờ VN +1) , Nhật (Giờ VN +2) , Argentina (Giờ VN -10). Ký tự nơi ở sẽ thấy kí tự theo Mã chung ISO và cách 15 px, Giờ + thì > 24, Giờ - thì < 0)
  set_bg_color(COLOR_WHITE);
  fill_screen_bg();
  set_fg_color(COLOR_BLACK);
text_out_center("WORLD CLOCK", 88, 6);
text_out_center("TONY", 88, 30);

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 54, 172, 85);
  set_bg_color(COLOR_BLACK);
  text_out_center("VNM: ", 60, 58);
  char clock_time_VN[8]; 			//	текст время		12:34
		struct datetime_ dt;
		get_current_date_time(&dt);
		_sprintf(clock_time_VN, "%02d:%02d", dt.hour, dt.min);
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_VN, 80 , 58, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);     


  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 90, 86, 133);
  set_bg_color(COLOR_BLACK);
  text_out_center("RUS: ", 23, 104);
  char clock_time_RU[8]; 			//	текст время		12:34
		
		 if (dt.hour - 4 < 0) {
  _sprintf(clock_time_RU, "%02d:%02d", dt.hour - 4 + 24, dt.min);
 }
  else {
 _sprintf(clock_time_RU, "%02d:%02d", dt.hour - 4, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_RU, 38 , 104, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);     

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(90, 90, 172, 133);
  set_bg_color(COLOR_BLACK);
  text_out_center("JPN: ", 110, 104);
    char clock_time_JP[8]; 			//	текст время		12:34
		
		 if (dt.hour + 2 > 24) {
  _sprintf(clock_time_JP, "%02d:%02d", dt.hour + 2 - 24, dt.min);
 }
  else {
 _sprintf(clock_time_JP, "%02d:%02d", dt.hour + 2, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_JP, 125 , 104, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);    
  
    set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 140, 86, 169);
  set_bg_color(COLOR_BLACK);
  text_out_center("CHN: ", 23, 147);
      char clock_time_CN[8]; 			//	текст время		12:34
		
		 if (dt.hour + 1 > 24) {
  _sprintf(clock_time_CN, "%02d:%02d", dt.hour + 1 - 24, dt.min);
 }
  else {
 _sprintf(clock_time_CN, "%02d:%02d", dt.hour + 1, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_CN, 38 , 147, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176); 

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(90, 140, 172, 169);
  set_bg_color(COLOR_BLACK);
  text_out_center("ARG: ", 110, 147); //ARGENTINA
      char clock_time_AR[8]; 			//	текст время		12:34
		
		 if (dt.hour - 10 < 0) {
  _sprintf(clock_time_AR, "%02d:%02d", dt.hour - 10 + 24, dt.min);
 }
  else {
 _sprintf(clock_time_AR, "%02d:%02d", dt.hour - 10, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_AR, 125 , 147, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176); 


  repaint_screen_lines(0, 176);
 
}

void draw_score_screen(){
  set_fg_color(COLOR_WHITE);
  set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  set_fg_color(COLOR_WHITE);
text_out_center("Xin chao Tony Nguyen", 88, 6);

  set_bg_color(COLOR_GREEN);
  set_fg_color(COLOR_BLACK);
  draw_filled_rect_bg(4, 54, 172, 85);
  set_bg_color(COLOR_GREEN);
  text_out_center("TONY", 88, 58);


  set_bg_color(COLOR_YELLOW);
  set_fg_color(COLOR_BLACK);
  draw_filled_rect_bg(4, 90, 86, 133);
  set_bg_color(COLOR_YELLOW);
  text_out_center("CAPCUU", 40, 104);

  set_bg_color(COLOR_AQUA);
  set_fg_color(COLOR_BLACK);
  draw_filled_rect_bg(90, 90, 172, 133);
  set_bg_color(COLOR_AQUA);
  text_out_center("CHON LUA", 132, 104);
  
    set_bg_color(COLOR_BLUE);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 140, 86, 169);
  set_bg_color(COLOR_BLUE);
  text_out_center("SSKDT", 42, 147);

  set_bg_color(COLOR_RED);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(90, 140, 172, 169);
  set_bg_color(COLOR_RED);
  text_out_center("PC COVID", 132, 147);


  repaint_screen_lines(0, 176);
 
}


void ask_confirmation(){
  	//struct app_data_**  app_data_p = (struct app_data_ **)get_ptr_temp_buf_2();    //  указатель на указатель на данные экрана
	//struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана
	set_bg_color(COLOR_BLACK); // Set mau nen
fill_screen_bg(); 
//set_graph_callback_to_ram_1(); 
load_font(); 
    show_statusbar(3, COLOR_BLACK,COLOR_WHITE);	// статус бар
	
	char crnd[8];     // переменная для перевода переменной col типа int в  тип char для печати                               
    _sprintf(crnd, "%07d", COLOR_WHITE); // конвертируем число int в char лидирующие нули, при необходимости, _sprintf добьёт сам
	set_bg_color(COLOR_BLACK); // делаем фон черным
	draw_filled_rect_bg(0, 43, 176, 110); // стираем предыдущее значение
    set_fg_color(COLOR_WHITE);
	text_out("Tony: 201.753.760 \n091.875.2778 \nSV 479.482.016.9604 \nBa: 091.349.4576 \nhttp://bit.do/bateo \nMe: 090.529.5788 \nm.me/tong.t.luu", 5, 30);

  repaint_screen_lines(0, 176);

}

void chon_lua(){
set_bg_color(COLOR_BLUE); // Set mau nen
fill_screen_bg(); 
//set_graph_callback_to_ram_1(); 
load_font(); 
set_fg_color(COLOR_YELLOW); 
struct datetime_ dt;
		get_current_date_time(&dt);
		int sum1 = dt.hour+dt.min+dt.sec;
if (sum1 % 3 == 0 ) 
{
	text_out("Lam lai", 50, 3); 
}
else if(sum1 % 3 == 1 ){
        text_out("Cho phep", 50, 3);
}
else
{
	text_out("Khong duoc", 50, 3); 
}
char crnd1[4]; 
set_fg_color(COLOR_AQUA); // Set mau chu
_sprintf(crnd1, "%d", sum1); 
text_out_center(crnd1, 20, 3); 
set_fg_color(COLOR_WHITE); 
text_out_center("Tony Nguyen", 88, 23); 
text_out_center("Chon lua cuoc song", 88, 43); 
text_out_center("Dua vao 3 so dau \nSo cuoi de lo to", 88, 63); 
_srand(get_tick_count()); 
int rnd1=_rand()%100; // Random so tu 0 den 9
int rnd2=_rand()%100; // Random so tu 0 den 9
int rnd3=_rand()%100; // Random so tu 0 den 99
int rnd4=_rand()%5; // Random so tu 0 den 4, random chon dap an
char crnd[4]; 
set_fg_color(COLOR_AQUA); // Set mau chu
_sprintf(crnd, "%d", rnd1); 
text_out_center(crnd, 35, 110); 
_sprintf(crnd, "%d", rnd2); 
text_out_center(crnd, 70, 110); 
_sprintf(crnd, "%d", rnd3); 
text_out_center(crnd, 105, 110); 
_sprintf(crnd, "%d", rnd4); 
text_out_center(crnd, 140, 110);
int sum = rnd1+rnd2+rnd3;
if (sum % 3 == 0 ) 
{
	text_out_center("Lam lai", 88, 130); 
}
else if(sum % 3 == 1 ){
        text_out_center("Cho phep lam", 88, 130);
}
else
{
	text_out_center("Khong duoc lam", 88, 130); 
}
set_fg_color(COLOR_RED); 
text_out_center("Tap play", 88, 150); 

  repaint_screen_lines(0, 176);

}


void draw_screen(){
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана
 set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  load_font();
  //set_graph_callback_to_ram_1();
  draw_board();
  
  //set_graph_callback_to_ram_1();
}


