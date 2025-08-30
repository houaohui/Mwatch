
    AREA |.text|, CODE, READONLY, ALIGN=2
    THUMB
    REQUIRE8
    PRESERVE8

; api函数表在地址
table_ptr EQU (0x2000C000 - 4)    ; API表基地址

	MACRO
	api_item $name_, $index_, $type_
    EXPORT $name_
$name_ PROC
    LDR r12, =table_ptr           ; 加载API表指针地址
    LDR r12, [r12]                ; 获取API表实际地址
    
    IF "$type_" == "FUNC"
        ; 函数处理：跳转到目标函数
        LDR r12, [r12, #4*$index_]
        BX r12
    ELSE
        ; 变量处理：返回变量指针
        LDR r0, [r12, #4*$index_] ; 直接加载变量指针值
        BX lr
    ENDIF
	ENDP
	MEND


; 定义系统api函数/变量
; 操作系统内核相关api
	api_item alarm_getNext,1,FUNC ;
	api_item alarm_getNextDay,2,FUNC ;
	api_item appConfig_sysVar,3,VAR  ;
	api_item colon_sysVar,4,VAR  ;
	api_item display_setDrawFunc,5,FUNC ;
	api_item div10,6,FUNC ;
	api_item dowImg_sysVar,7,VAR  ;
	api_item drawBattery,8,FUNC ;
	api_item drawDate,9,FUNC ;
	api_item drawTemperature,10,FUNC ;
	api_item draw_bitmap,11,FUNC ;
	api_item draw_string,12,FUNC ;
	api_item millis_get,13,FUNC  ;
	api_item mod10,14,FUNC ;
	api_item openLSM6DSM,15,FUNC ;
	api_item open_watchface_general,16,FUNC ;
	api_item stopwatch_sysVar,17,VAR  ;
	api_item stopwatch_active,18,FUNC ;
	api_item timeDate_sysVar,19,VAR  ;
	api_item time_timeMode,20,FUNC ;
	api_item watchFace_switchUpdate,21,FUNC ;
	api_item console_log,22,FUNC ;
	api_item drawVectorChar,23,FUNC ;
	api_item setVectorDrawClipWin,24,FUNC ;
	api_item getBtLinkState,25,FUNC ;
	api_item getCharingState,26,FUNC ;
	api_item blueToothIcon_sysVar,27,VAR ;
	api_item chargeIcon_sysVar,28,VAR ;
	api_item myOLED_DrawLine,29,FUNC ;
	api_item smallFont_sysVar,30,VAR ;
	api_item ticker,31,FUNC ;
	api_item draw_point,32,FUNC ;
	; ...省略其它api定义...

    ALIGN   4
    END

