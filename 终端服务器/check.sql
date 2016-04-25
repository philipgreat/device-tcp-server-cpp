
alter session set NLS_DATE_FORMAT='mmdd hh24:mi:ss';
set linesize 1000;
select ID||'|'||IMEI_NUMBER||'|'||UIM_NUMBER||'|'|| MSISDN||'|'|| HOTEL_ID||'|'||ROOM_NO||'|'||BROKER_ID||'|'||START_TIME||'|'||HIRE_SRC||'|'||HIRE_CONFIRM_OPErator||'|'||HIRE_CONFIRM_TIME||'|'||HIRE_NOTES||'|'||END_TIME||'|'||QUIT_SRC||'|'||QUIT_NOTES||'|'||STATUS from t_sdr_200902 order by id;

select imei_number||'|'||uim_number||'|'||msisdn||'|'||status||'|'||last_hire_time||'|'||last_quit_hire_time||'|'||hire_request_time||'|'||current_hire_time||'|'||hotel_id||'|'||hotel_receiver||'|'||content_version||'|'||room_no||'|'||last_hb_time from T_Terminal_Info;


exit;
