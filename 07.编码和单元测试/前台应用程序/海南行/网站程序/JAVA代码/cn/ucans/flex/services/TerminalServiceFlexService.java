package cn.ucans.flex.services;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


import chobits.common.DateUtil;
import chobits.db.DBBuilder;
import chobits.db.DBFactory;
import chobits.log.Logger;
import cn.ucans.bean.T_ORG_INFO;
import cn.ucans.bean.T_SDR;
import cn.ucans.bean.T_TERMINAL_INFO;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class TerminalServiceFlexService extends FlexService{
	
	private Logger logger = new Logger(TerminalServiceFlexService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------终端管理任务 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 1200:
					reponse = this.initAdd00ATerminalPageParm(request);
					break;
				case 1201:
					reponse = this.add00ATerminal(request);
					break;
				case 1202:
					reponse = this.update00ATerminal(request);
					break;
				case 1203:
					reponse = this.initQuery00ATerminalPageParm(request);
					break;
				case 1204:
					reponse = this.query00ATerminal(request);
					break;
				case 1205:
					reponse = this.queryPage00ATerminal(request);
					break;
				case 1206:
					reponse = this.fenFa00ATerminal(request);
					break;
				case 1207:
					reponse = this.recover00ATerminal(request);
					break;
				case 1208:
					reponse = this.revoke00ATerminal(request);
					break;
				case 1209:
					reponse = this.query00ALeadData(request);
					break;
				case 1210:
					reponse = this.lead00AData(request);
					break;
				case 1220:
					reponse = this.initQueryHotelTerminalPageParm(request);
					break;
				case 1221:
					reponse = this.queryPageHotelTerminal(request);
					break;
				case 1222:
					reponse = this.readyOpenTerminal(request);
					break;
				case 1223:
					reponse = this.cancleReadyOpenTerminal(request);
					break;
				case 1224:
					reponse = this.backHireTerminal(request);
					break;
				case 1225:
					reponse = this.updateHotelTerminal(request);
					break;
				case 1226:
					reponse = this.queryPageSDRViewTable(request);
					break;
				case 1227:
					reponse = this.doSDRTerminalConfirmInfo(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------终端管理任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------终端管理任务 执行失败-------------------------------");
		}		
		return reponse;
	}
	
	private FlexReponse initAdd00ATerminalPageParm(FlexRequest request) throws Exception{
		Map<String,Object> map = new HashMap<String,Object>();
		map.put("label", "全部");
		map.put("data", "");
		ParamHandler handler = new ParamHandler();
		List<Map<String,Object>> list_org_id = handler.getT_Org_Info_Org_ID_Org_Name("('1')","('0')");
		List<Map<String,Object>> list_terminal_status = handler.getT_Terminal_Info_Status(null,null);
		List<Map<String,Object>> list_terminal_type_edit = handler.getT_Terminal_Type();
		List<Map<String,Object>> list_terminal_type_query = handler.getT_Terminal_Type();
		list_terminal_type_query.add(0, map);
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list_org_id", list_org_id);
		reponse.set("list_terminal_status", list_terminal_status);
		reponse.set("list_terminal_type_edit", list_terminal_type_edit);
		reponse.set("list_terminal_type_query", list_terminal_type_query);
		return reponse;
	}
	
	private FlexReponse initQuery00ATerminalPageParm(FlexRequest request) throws Exception{
		Map<String,Object> map = new HashMap<String,Object>();
		map.put("label", "全部");
		map.put("data", "");
		ParamHandler handler = new ParamHandler();
		List<Map<String,Object>> list_factory_edit = handler.getT_Org_Info_Org_ID_Org_Name("('1')","('0')");
		List<Map<String,Object>> list_factory_query = handler.getT_Org_Info_Org_ID_Org_Name("('1')","('0')");
		List<Map<String,Object>> list_hotel_query = handler.getT_Org_Info_Org_ID_Org_Name("('3')","('0')");
		List<Map<String,Object>> list_hotel_edit = handler.getT_Org_Info_Org_ID_Org_Name("('3')","('0')");
		List<Map<String,Object>> list_terminal_status_edit = handler.getT_Terminal_Info_Status(null,null);
		List<Map<String,Object>> list_terminal_status_query = handler.getT_Terminal_Info_Status(null,null);
		List<Map<String,Object>> list_terminal_type_edit = handler.getT_Terminal_Type();
		List<Map<String,Object>> list_terminal_type_query = handler.getT_Terminal_Type();
		list_terminal_type_query.add(0, map);
		list_factory_query.add(0, map);
		list_hotel_query.add(0, map);
		list_terminal_status_query.add(0, map);
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list_factory_edit", list_factory_edit);
		reponse.set("list_factory_query", list_factory_query);
		reponse.set("list_hotel_query", list_hotel_query);
		reponse.set("list_hotel_edit", list_hotel_edit);
		reponse.set("list_terminal_status_edit", list_terminal_status_edit);
		reponse.set("list_terminal_status_query", list_terminal_status_query);
		reponse.set("list_terminal_type_edit", list_terminal_type_edit);
		reponse.set("list_terminal_type_query", list_terminal_type_query);
		return reponse;
	}
	
	private FlexReponse add00ATerminal(FlexRequest request) throws Exception{
		T_TERMINAL_INFO terminal = (T_TERMINAL_INFO) request.getBean("terminal");
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		int count = factory.getCount("T_TERMINAL_INFO", "Imei_Number='"+terminal.IMEI_NUMBER+"' and Uim_Number='"+terminal.UIM_NUMBER+"'");
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("终端管理，添加终端信息，校验Imei编号和Uim编号，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}else{
			if(count>0){
				factory.interruptTransaction();
				throw new Exception("终端管理，添加终端信息，该终端（Imei编号"+terminal.IMEI_NUMBER+"，Uim编号"+terminal.UIM_NUMBER+"）已经存在！");
			}
		}
		
		Map<String,Object> map_org_id = factory.queryMap("select Org_ID from T_Terminal_Type where Terminal_Type='"+terminal.TERMINAL_TYPE+"'");
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("终端管理，添加终端信息，校验终端生产厂商及其型号出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}else{
			if(map_org_id == null){
				factory.interruptTransaction();
				throw new Exception("终端管理，添加终端信息，该终端的校验终端生产厂商及其型号（生产厂商"+terminal.PRODUCT_ORG_ID+"，型号"+terminal.TERMINAL_TYPE+"）不存在！");
			}else{
				terminal.PRODUCT_ORG_ID = (String) map_org_id.get("ORG_ID");
			}
		}
		
		String sql_insert_terminal = "insert into T_TERMINAL_INFO (IMEI_NUMBER,UIM_NUMBER,MSISDN,PRODUCT_ORG_ID,TERMINAL_TYPE,PRODUCT_DATE,MAINTAIN_LIMITED_DATE,CONTENT_VERSION,Status,Join_Time,Join_User_ID) values ";
		sql_insert_terminal += "('"+terminal.IMEI_NUMBER+"','"+terminal.UIM_NUMBER+"','"+terminal.MSISDN+"','"+terminal.PRODUCT_ORG_ID+"','"+terminal.TERMINAL_TYPE+"','"+terminal.PRODUCT_DATE.replaceAll("-", "")+"','"+terminal.MAINTAIN_LIMITED_DATE.replaceAll("-", "")+"','"+terminal.CONTENT_VERSION+"',0,sysdate,'"+request.user_id+"')";
		factory.excute(sql_insert_terminal);
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("终端管理，添加终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		String sql_insert_his = "insert into T_Terminal_Change_His (Imei_Number,Uim_Number,Msisdn,Change_Type,User_ID,Change_Time) values ";
		sql_insert_his += "('"+terminal.IMEI_NUMBER+"','"+terminal.UIM_NUMBER+"','"+terminal.MSISDN+"','01','"+request.user_id+"',sysdate)";
		factory.excute(sql_insert_his);
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("终端管理，更新终端变更信息表出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("终端管理，提交添加终端信息事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"添加终端（Imei编号"+terminal.IMEI_NUMBER+"，Uim编号"+terminal.UIM_NUMBER+"，"+terminal.MSISDN+"）信息成功！");
		return reponse;
	}
	
	private FlexReponse update00ATerminal(FlexRequest request) throws Exception{
		T_TERMINAL_INFO terminal = (T_TERMINAL_INFO) request.getBean("terminal");
		Map<String,Object> old_map = (Map<String,Object>)request.getBean("item");
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		int count = factory.getCount("T_TERMINAL_INFO", "Imei_Number='"+terminal.IMEI_NUMBER+"' and Uim_Number='"+terminal.UIM_NUMBER+"'");
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("客户服务，更新终端信息，校验Imei编号和Uim编号，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}else{
			if(count==0){
				factory.interruptTransaction();
				throw new Exception("客户服务，更新终端厂商信息，该终端（Imei编号"+terminal.IMEI_NUMBER+"，Uim编号"+terminal.UIM_NUMBER+"）已经不存在！");
			}
		}
		
		Map<String,Object> map_org_id = factory.queryMap("select Org_ID from T_Terminal_Type where Terminal_Type='"+terminal.TERMINAL_TYPE+"'");
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("终端管理，添加终端信息，校验终端生产厂商及其型号出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}else{
			if(map_org_id == null){
				factory.interruptTransaction();
				throw new Exception("终端管理，添加终端信息，该终端的校验终端生产厂商及其型号（生产厂商"+terminal.PRODUCT_ORG_ID+"，型号"+terminal.TERMINAL_TYPE+"）不存在！");
			}else{
				terminal.PRODUCT_ORG_ID = (String) map_org_id.get("ORG_ID");
			}
		}
		
		String sql_update = "update T_Terminal_Info set ";
		sql_update += "CONTENT_VERSION = '"+terminal.CONTENT_VERSION+"',";
		sql_update += "TERMINAL_TYPE = '"+terminal.TERMINAL_TYPE+"',";
		sql_update += "PRODUCT_DATE = '"+terminal.PRODUCT_DATE.replaceAll("-", "")+"',";
		sql_update += "MAINTAIN_LIMITED_DATE = '"+terminal.MAINTAIN_LIMITED_DATE.replaceAll("-", "")+"',";
		sql_update += "HOTEL_RECEIVER = '"+terminal.HOTEL_RECEIVER+"' ";
		sql_update += " where IMEI_NUMBER='"+terminal.IMEI_NUMBER+"' and Uim_Number='"+terminal.UIM_NUMBER+"'";
		factory.excute(sql_update);
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("终端管理，变更终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		String hotel_id = (String) old_map.get("HOTEL_ID");
		String Content_Version = (String) old_map.get("CONTENT_VERSION");
		String sql = "";
		//05终端厂家变化;06终端型号变化;07内容版本变化;08酒店领用人变化;09房间号变化
		if(!Content_Version.equals(terminal.CONTENT_VERSION)){
			sql = "insert into T_Terminal_Change_His (Imei_Number,Uim_Number,Msisdn,Change_Type,Hotel_ID,User_ID,Change_Time,Old_Values,New_Values) values ";
			sql += "('"+terminal.IMEI_NUMBER+"','"+terminal.UIM_NUMBER+"','"+terminal.MSISDN+"','07','"+hotel_id+"','"+request.user_id+"',sysdate,'"+Content_Version+"','"+terminal.CONTENT_VERSION+"')";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){			
				throw new Exception("终端管理，增加终端变更信息表（版本变化）出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
		}
		String Terminal_Type = (String) old_map.get("TERMINAL_TYPE");
		if(!Terminal_Type.equals(terminal.TERMINAL_TYPE)){
			sql = "insert into T_Terminal_Change_His (Imei_Number,Uim_Number,Msisdn,Change_Type,Hotel_ID,User_ID,Change_Time,Old_Values,New_Values) values ";
			sql += "('"+terminal.IMEI_NUMBER+"','"+terminal.UIM_NUMBER+"','"+terminal.MSISDN+"','06','"+hotel_id+"','"+request.user_id+"',sysdate,'"+Terminal_Type+"','"+terminal.TERMINAL_TYPE+"')";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){			
				throw new Exception("终端管理，增加终端变更信息表（终端型号）出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
		}
		String Product_Date = (String) old_map.get("PRODUCT_DATE");
		if(!Product_Date.equals(terminal.PRODUCT_DATE)){
			sql = "insert into T_Terminal_Change_His (Imei_Number,Uim_Number,Msisdn,Change_Type,Hotel_ID,User_ID,Change_Time,Old_Values,New_Values) values ";
			sql += "('"+terminal.IMEI_NUMBER+"','"+terminal.UIM_NUMBER+"','"+terminal.MSISDN+"','00','"+hotel_id+"','"+request.user_id+"',sysdate,'"+Product_Date+"','"+terminal.PRODUCT_DATE+"')";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){			
				throw new Exception("终端管理，增加终端变更信息表（生产日期）出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
		}
		String Maintain_Limited_Date = (String) old_map.get("MAINTAIN_LIMITED_DATE");
		if(!Maintain_Limited_Date.equals(terminal.MAINTAIN_LIMITED_DATE)){
			sql = "insert into T_Terminal_Change_His (Imei_Number,Uim_Number,Msisdn,Change_Type,Hotel_ID,User_ID,Change_Time,Old_Values,New_Values) values ";
			sql += "('"+terminal.IMEI_NUMBER+"','"+terminal.UIM_NUMBER+"','"+terminal.MSISDN+"','00','"+hotel_id+"','"+request.user_id+"',sysdate,'"+Maintain_Limited_Date+"','"+terminal.MAINTAIN_LIMITED_DATE+"')";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){			
				throw new Exception("终端管理，增加终端变更信息表（维修截止日期）出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
		}
		String Hotel_Receiver = (String) old_map.get("HOTEL_RECEIVER");
		if(!Hotel_Receiver.equals(terminal.HOTEL_RECEIVER)){
			sql = "insert into T_Terminal_Change_His (Imei_Number,Uim_Number,Msisdn,Change_Type,Hotel_ID,User_ID,Change_Time,Old_Values,New_Values) values ";
			sql += "('"+terminal.IMEI_NUMBER+"','"+terminal.UIM_NUMBER+"','"+terminal.MSISDN+"','08','"+hotel_id+"','"+request.user_id+"',sysdate,'"+Hotel_Receiver+"','"+terminal.HOTEL_RECEIVER+"')";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){			
				throw new Exception("终端管理，增加终端变更信息表（酒店领用人）出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
		}
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("终端管理，更新终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"更新终端信息成功！");
		return reponse;
	}
	
	private FlexReponse updateHotelTerminal(FlexRequest request) throws Exception{
		T_TERMINAL_INFO terminal = (T_TERMINAL_INFO) request.getBean("terminal");
		Map<String,Object> old_map = (Map<String,Object>)request.getBean("item");
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		int count = factory.getCount("T_TERMINAL_INFO", "Imei_Number='"+terminal.IMEI_NUMBER+"' and Uim_Number='"+terminal.UIM_NUMBER+"'");
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("客户服务，更新终端信息，校验Imei编号和Uim编号，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}else{
			if(count==0){
				factory.interruptTransaction();
				throw new Exception("客户服务，更新终端厂商信息，该终端（Imei编号"+terminal.IMEI_NUMBER+"，Uim编号"+terminal.UIM_NUMBER+"）已经不存在！");
			}
		}
		
		Map<String,Object> map_org_id = factory.queryMap("select Org_ID from T_Terminal_Type where Terminal_Type='"+terminal.TERMINAL_TYPE+"'");
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("终端管理，添加终端信息，校验终端生产厂商及其型号出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}else{
			if(map_org_id == null){
				throw new Exception("终端管理，添加终端信息，该终端的校验终端生产厂商及其型号（生产厂商"+terminal.PRODUCT_ORG_ID+"，型号"+terminal.TERMINAL_TYPE+"）不存在！");
			}else{
				terminal.PRODUCT_ORG_ID = (String) map_org_id.get("ORG_ID");
			}
		}
		
		String sql_update = "update T_Terminal_Info set ";
		sql_update += "CONTENT_VERSION = '"+terminal.CONTENT_VERSION+"',";
		sql_update += "Room_No = '"+terminal.ROOM_NO+"',";
		sql_update += "HOTEL_RECEIVER = '"+terminal.HOTEL_RECEIVER+"' ";
		sql_update += " where IMEI_NUMBER='"+terminal.IMEI_NUMBER+"' and Uim_Number='"+terminal.UIM_NUMBER+"'";
		factory.excute(sql_update);
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("终端管理，变更终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		String hotel_id = (String) old_map.get("HOTEL_ID");
		String Content_Version = (String) old_map.get("CONTENT_VERSION");
		String sql = "";
		//05终端厂家变化;06终端型号变化;07内容版本变化;08酒店领用人变化;09房间号变化
		if(!Content_Version.equals(terminal.CONTENT_VERSION)){
			sql = "insert into T_Terminal_Change_His (Imei_Number,Uim_Number,Msisdn,Change_Type,Hotel_ID,User_ID,Change_Time,Old_Values,New_Values) values ";
			sql += "('"+terminal.IMEI_NUMBER+"','"+terminal.UIM_NUMBER+"','"+terminal.MSISDN+"','07','"+hotel_id+"','"+request.user_id+"',sysdate,'"+Content_Version+"','"+terminal.CONTENT_VERSION+"')";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){			
				throw new Exception("终端管理，增加终端变更信息表（版本变化）出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
		}
		String Hotel_Receiver = (String) old_map.get("HOTEL_RECEIVER");
		if(!Hotel_Receiver.equals(terminal.HOTEL_RECEIVER)){
			sql = "insert into T_Terminal_Change_His (Imei_Number,Uim_Number,Msisdn,Change_Type,Hotel_ID,User_ID,Change_Time,Old_Values,New_Values) values ";
			sql += "('"+terminal.IMEI_NUMBER+"','"+terminal.UIM_NUMBER+"','"+terminal.MSISDN+"','08','"+hotel_id+"','"+request.user_id+"',sysdate,'"+Hotel_Receiver+"','"+terminal.HOTEL_RECEIVER+"')";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){			
				throw new Exception("终端管理，增加终端变更信息表（酒店领用人）出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
		}
		String Room_No = (String) old_map.get("ROOM_NO");
		if(!Room_No.equals(terminal.ROOM_NO)){
			sql = "insert into T_Terminal_Change_His (Imei_Number,Uim_Number,Msisdn,Change_Type,Hotel_ID,User_ID,Change_Time,Old_Values,New_Values) values ";
			sql += "('"+terminal.IMEI_NUMBER+"','"+terminal.UIM_NUMBER+"','"+terminal.MSISDN+"','09','"+hotel_id+"','"+request.user_id+"',sysdate,'"+Room_No+"','"+terminal.ROOM_NO+"')";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){			
				throw new Exception("终端管理，增加终端变更信息表（酒店领用人）出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
		}
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("终端管理，更新终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"更新终端信息成功！");
		return reponse;
	}
	
	private FlexReponse delete00ATerminal(FlexRequest request) throws Exception{
		T_ORG_INFO org = (T_ORG_INFO) request.getBean("org");
		DBBuilder builder = new DBBuilder();
		
		String delete_sql = "update T_ORG_INFO set Status='1' where Org_ID='"+org.ORG_ID+"' and Status='1' and Org_Type='1'";
		builder.excute(delete_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("客户服务，删除终端厂商信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"删除终端厂商（"+org.ORG_NAME+"）信息成功！");
		return reponse;
	}
	
	private FlexReponse query00ATerminal(FlexRequest request) throws Exception{
		T_TERMINAL_INFO terminal = (T_TERMINAL_INFO) request.getBean("terminal");
		String notbefore = (String) request.getBean("notbefore");
		String notafter = (String) request.getBean("notafter");
		
		DBBuilder builder = new DBBuilder();	
		String query_sql = "select IMEI_NUMBER,UIM_NUMBER,MSISDN,TERMINAL_TYPE,STATUS,JOIN_TIME,JOIN_USER_ID from T_TERMINAL_INFO ";
		query_sql += " where Join_Time >= to_date('"+notbefore+"','yyyy-mm-dd hh24:mi:ss') and Join_Time <= to_date('"+notafter+"','yyyy-mm-dd hh24:mi:ss')";
		if(terminal.IMEI_NUMBER != null && terminal.IMEI_NUMBER.trim().length()>0){
			query_sql += "  and IMEI_NUMBER = '"+terminal.IMEI_NUMBER+"'";
		}
		if(terminal.UIM_NUMBER != null && terminal.UIM_NUMBER.trim().length()>0){
			query_sql += "  and UIM_NUMBER = '"+terminal.UIM_NUMBER+"'";
		}
		if(terminal.MSISDN != null && terminal.MSISDN.trim().length()>0){
			query_sql += "  and MSISDN = '"+terminal.MSISDN+"'";
		}
		if(terminal.TERMINAL_TYPE != null && terminal.TERMINAL_TYPE.trim().length()>0){
			query_sql += "  and TERMINAL_TYPE = '"+terminal.TERMINAL_TYPE+"'";
		}
		
		if(terminal.ROOM_NO != null && terminal.ROOM_NO.trim().length()>0){
			query_sql += "  and ROOM_NO = '"+terminal.ROOM_NO+"'";
		}
		
		query_sql += " order by Join_Time";
		List<Map<String,Object>> list = builder.queryList(query_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("终端管理服务，查询终端信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse queryPage00ATerminal(FlexRequest request) throws Exception{
		T_TERMINAL_INFO terminal = (T_TERMINAL_INFO) request.getBean("terminal");
		String CURRENT_PAGE_NO = (String) request.getBean("CURRENT_PAGE_NO");
		
		DBBuilder builder = new DBBuilder();			
		
		String field = "Imei_Number,Uim_Number,Msisdn,Status,Last_Hire_Time,Last_Quit_Hire_Time,Hire_Request_Time,Current_Hire_Time,Product_Org_ID,Terminal_Type,Product_Date,Maintain_Limited_Date,Join_Time,Join_User_ID,Distribute_Time,Distribute_User_ID,Hotel_ID,Hotel_Receiver,Content_Version,Room_No,Language,Last_HB_Time";
		String table = " T_TERMINAL_INFO ";
		String solution = "";
		boolean haswhere = false;
		if(terminal.IMEI_NUMBER != null && terminal.IMEI_NUMBER.trim().length()>0){
			solution += "  where IMEI_NUMBER = '"+terminal.IMEI_NUMBER+"'";
			haswhere = true;
		}
		if(terminal.UIM_NUMBER != null && terminal.UIM_NUMBER.trim().length()>0){
			if(haswhere){
				solution += "  and UIM_NUMBER = '"+terminal.UIM_NUMBER+"'";
			}else{
				solution += "  where UIM_NUMBER = '"+terminal.UIM_NUMBER+"'";
				haswhere = true;
			}
			
		}
		if(terminal.MSISDN != null && terminal.MSISDN.trim().length()>0){
			if(haswhere){
				solution += "  and MSISDN = '"+terminal.MSISDN+"'";
			}else{
				solution += "  where MSISDN = '"+terminal.MSISDN+"'";
				haswhere = true;
			}
			
		}	
		if(terminal.HOTEL_ID != null && terminal.HOTEL_ID.trim().length()>0){
			if(haswhere){
				solution += "  and HOTEL_ID = '"+terminal.HOTEL_ID+"'";
			}else{
				solution += "  where HOTEL_ID = '"+terminal.HOTEL_ID+"'";
				haswhere = true;
			}
			
		}	
		if(terminal.PRODUCT_ORG_ID != null && terminal.PRODUCT_ORG_ID.trim().length()>0){
			if(haswhere){
				solution += "  and PRODUCT_ORG_ID = '"+terminal.PRODUCT_ORG_ID+"'";
			}else{
				solution += "  where PRODUCT_ORG_ID = '"+terminal.PRODUCT_ORG_ID+"'";
				haswhere = true;
			}
			
		}	
		
		if(terminal.ROOM_NO != null && terminal.ROOM_NO.trim().length()>0){
			if(haswhere){
				solution += "  and ROOM_NO = '"+terminal.ROOM_NO+"'";
			}else{
				solution += "  where ROOM_NO = '"+terminal.ROOM_NO+"'";
				haswhere = true;
			}
			
		}	
		
		if(terminal.TERMINAL_TYPE != null && terminal.TERMINAL_TYPE.trim().length()>0){
			if(haswhere){
				solution += "  and TERMINAL_TYPE = '"+terminal.TERMINAL_TYPE+"'";
			}else{
				solution += "  where TERMINAL_TYPE = '"+terminal.TERMINAL_TYPE+"'";
				haswhere = true;
			}
			
		}	
		if(terminal.STATUS != null && terminal.STATUS.trim().length()>0){
			if(haswhere){
				solution += "  and STATUS = "+terminal.STATUS+"";
			}else{
				solution += "  where STATUS = "+terminal.STATUS+"";
				haswhere = true;
			}
			
		}	
		solution += " order by Join_Time";
		Map<String,Object> map = builder.queryPage(field, table, solution, Integer.parseInt(CURRENT_PAGE_NO), false);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("终端管理服务，分页查询终端信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("map", map);
		return reponse;
	}
	
	private FlexReponse queryPageSDRViewTable(FlexRequest request) throws Exception{
		T_SDR sdr = (T_SDR) request.getBean("sdr");
		String CURRENT_PAGE_NO = (String) request.getBean("CURRENT_PAGE_NO");
		String notbefore = (String) request.getBean("notbefore");
		String notafter = (String) request.getBean("notafter");
	
		DBBuilder builder = new DBBuilder();			
		String field = "ID,ROOM_NO,MSISDN,START_TIME,HIRE_SRC,HIRE_CONFIRM_OPERATOR,HIRE_CONFIRM_TIME";
		String table = " V_SDR ";
		String solution = " HOTEL_ID='"+request.org_id+"' and START_TIME >= to_date('"+notbefore+"','yyyy-mm-dd hh24:mi:ss') and START_TIME <= to_date('"+notafter+"','yyyy-mm-dd hh24:mi:ss') ";
		if(sdr.ROOM_NO != null && sdr.ROOM_NO.trim().length()>0){
			solution += " and ROOM_NO = '"+sdr.ROOM_NO+"' ";			
		}
		if(sdr.MSISDN != null && sdr.MSISDN.trim().length()>0){
			solution += " and MSISDN = '"+sdr.MSISDN+"' ";			
		}
		solution += " order by START_TIME";
		Map<String,Object> map = builder.queryPage(field, table, solution, Integer.parseInt(CURRENT_PAGE_NO), true);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("终端管理服务，分页查询SDR信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		DateUtil du = new DateUtil();
		String time = du.getYMDHMSTime(DateUtil.Format_EN_yyyy_MM_dd_HH_mm_ss);
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("map", map);
		reponse.set("time", time);
		return reponse;
	}
	
	private FlexReponse fenFa00ATerminal(FlexRequest request) throws Exception{
		String hotel_id = (String) request.getBean("hotel_id");
		String Receiver = (String) request.getBean("Receiver");
		ArrayList<Map<String,Object>> list = (ArrayList<Map<String, Object>>) request.getBean("list");
		List<Map<String,Object>> list_error = new ArrayList<Map<String, Object>>();
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		for(int i=0;i<list.size();i++){
			Map<String,Object> map = list.get(i);
			String IMEI_NUMBER = (String) map.get("IMEI_NUMBER");
			String UIM_NUMBER = (String) map.get("UIM_NUMBER");
			String MSISDN = (String) map.get("MSISDN");
			
			int count = factory.getCount("T_TERMINAL_INFO", "IMEI_NUMBER='"+IMEI_NUMBER+"' and UIM_NUMBER='"+UIM_NUMBER+"' and Status=0");
			if(factory.SQL_Error_Code != 0){		
				throw new Exception("终端管理服务，分发终端，判断当前终端是否符合条件出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			if(count == 0){
				list_error.add(map);
			}else{
				String sql_terminal = "update T_TERMINAL_INFO set Hotel_ID='"+hotel_id+"',Hotel_Receiver='"+Receiver+"',Distribute_Time=sysdate,Distribute_User_ID='"+request.user_id+"',Status=10 ";
				sql_terminal += " where IMEI_NUMBER='"+IMEI_NUMBER+"' and UIM_NUMBER='"+UIM_NUMBER+"' and Status=0";
				factory.excute(sql_terminal);
				if(factory.SQL_Error_Code != 0){		
					throw new Exception("终端管理服务，分发终端状态出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}
				String sql_his = "insert into T_Terminal_Change_His (Imei_Number,Uim_Number,Msisdn,Change_Type,Hotel_ID,User_ID,Change_Time,Old_Values,New_Values) values ";
				sql_his += "('"+IMEI_NUMBER+"','"+UIM_NUMBER+"','"+MSISDN+"','01','"+hotel_id+"','"+request.user_id+"',sysdate,'','"+hotel_id+"')";
				factory.excute(sql_his);
				if(factory.SQL_Error_Code != 0){		
					throw new Exception("终端管理服务，更新终端历史操作表，强行中断事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}
			}
		}		
		FlexReponse reponse = null;
		if(list_error.size()>0){
			factory.interruptTransaction();
			if(factory.SQL_Error_Code != 0){		
				throw new Exception("终端管理服务，检测出不符合操作条件的终端信息，强行中断事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			reponse = new FlexReponse(request,-100,"分发失败，检测出不符合条件的终端，没有改变任何终端信息！");
			reponse.set("list", list_error);
		}else{
			factory.closeTransaction();
			if(factory.SQL_Error_Code != 0){			
				throw new Exception("终端管理服务，分发终端终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			reponse = new FlexReponse(request,0,"分发"+list.size()+"个终端成功！");
		}
		return reponse;
	}
	
	private FlexReponse recover00ATerminal(FlexRequest request) throws Exception{
		ArrayList<Map<String,Object>> list = (ArrayList<Map<String, Object>>) request.getBean("list");
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		List<Map<String,Object>> list_error = new ArrayList<Map<String, Object>>();
		for(int i=0;i<list.size();i++){
			Map<String,Object> map = list.get(i);
			String IMEI_NUMBER = (String) map.get("IMEI_NUMBER");
			String UIM_NUMBER = (String) map.get("UIM_NUMBER");
			String MSISDN = (String) map.get("MSISDN");
			String HOTEL_ID = (String) map.get("HOTEL_ID");
			
			int count = factory.getCount("T_TERMINAL_INFO", "IMEI_NUMBER='"+IMEI_NUMBER+"' and UIM_NUMBER='"+UIM_NUMBER+"' and Status=10");
			if(factory.SQL_Error_Code != 0){		
				throw new Exception("终端管理服务，回收终端，判断当前终端是否符合条件出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			if(count == 0){
				list_error.add(map);
			}else{
				String sql_terminal = "update T_TERMINAL_INFO set Status=0,Hotel_ID='',Distribute_Time=null,Distribute_User_ID=''";
				sql_terminal += " where IMEI_NUMBER='"+IMEI_NUMBER+"' and UIM_NUMBER='"+UIM_NUMBER+"' and Status=10";
				int count_recover = factory.excute(sql_terminal);
				if(factory.SQL_Error_Code != 0){		
					throw new Exception("终端管理服务，回收终端状态出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}else{
					if(count_recover != 1){
						factory.interruptTransaction();
						throw new Exception("终端管理服务，回收终端个数（"+count_recover+"）超出限制！");
					}
				}
				String sql_his = "insert into T_Terminal_Change_His (Imei_Number,Uim_Number,Msisdn,Change_Type,Hotel_ID,User_ID,Change_Time,Old_Values,New_Values) values ";
				sql_his += "('"+IMEI_NUMBER+"','"+UIM_NUMBER+"','"+MSISDN+"','03','"+HOTEL_ID+"','"+request.user_id+"',sysdate,'','')";
				factory.excute(sql_his);
				if(factory.SQL_Error_Code != 0){		
					throw new Exception("终端管理服务，更新终端历史操作表，强行中断事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}
			}
		}
		FlexReponse reponse = null;
		if(list_error.size()>0){
			factory.interruptTransaction();
			if(factory.SQL_Error_Code != 0){		
				throw new Exception("终端管理服务，检测出不符合操作条件的终端信息，强行中断事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			reponse = new FlexReponse(request,-100,"回收失败，检测出不符合条件的终端，没有改变任何终端信息！");
			reponse.set("list", list_error);
		}else{
			factory.closeTransaction();
			if(factory.SQL_Error_Code != 0){			
				throw new Exception("终端管理服务，回收终端终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			reponse = new FlexReponse(request,0,"回收"+list.size()+"个终端成功！");
		}
		return reponse;
	}
	
	private FlexReponse revoke00ATerminal(FlexRequest request) throws Exception{
		ArrayList<Map<String,Object>> list = (ArrayList<Map<String, Object>>) request.getBean("list");
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		List<Map<String,Object>> list_error = new ArrayList<Map<String, Object>>();
		for(int i=0;i<list.size();i++){
			Map<String,Object> map = list.get(i);
			String IMEI_NUMBER = (String) map.get("IMEI_NUMBER");
			String UIM_NUMBER = (String) map.get("UIM_NUMBER");
			String MSISDN = (String) map.get("MSISDN");
			String HOTEL_ID = (String) map.get("HOTEL_ID");
			
			int count = factory.getCount("T_TERMINAL_INFO", "IMEI_NUMBER='"+IMEI_NUMBER+"' and UIM_NUMBER='"+UIM_NUMBER+"' and Status in (0,21,22,26,27)");
			if(factory.SQL_Error_Code != 0){		
				throw new Exception("终端管理服务，报废终端，判断当前终端是否符合条件出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			if(count == 0){
				list_error.add(map);
			}else{
				String sql_terminal = "update T_TERMINAL_INFO set Status=60 ";
				sql_terminal += " where IMEI_NUMBER='"+IMEI_NUMBER+"' and UIM_NUMBER='"+UIM_NUMBER+"' and Status in (0,21,22,26,27)";
				int count_revoke = factory.excute(sql_terminal);
				if(factory.SQL_Error_Code != 0){		
					throw new Exception("终端管理服务，作废终端状态出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}else{
					if(count_revoke != 1){
						factory.interruptTransaction();
						throw new Exception("终端管理服务，报废终端个数（"+count_revoke+"）超出限制！");
					}
				}
				String sql_his = "insert into T_Terminal_Change_His (Imei_Number,Uim_Number,Msisdn,Change_Type,Hotel_ID,User_ID,Change_Time,Old_Values,New_Values) values ";
				sql_his += "('"+IMEI_NUMBER+"','"+UIM_NUMBER+"','"+MSISDN+"','04','"+HOTEL_ID+"','"+request.user_id+"',sysdate,'','')";
				factory.excute(sql_his);
				if(factory.SQL_Error_Code != 0){		
					throw new Exception("终端管理服务，更新终端历史操作表，强行中断事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}
			}
		}
		FlexReponse reponse = null;
		if(list_error.size()>0){
			factory.interruptTransaction();
			if(factory.SQL_Error_Code != 0){		
				throw new Exception("终端管理服务，检测出不符合操作条件的终端信息，强行中断事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			reponse = new FlexReponse(request,-100,"报废失败，检测出不符合条件的终端，没有改变任何终端信息！");
			reponse.set("list", list_error);
		}else{
			factory.closeTransaction();
			if(factory.SQL_Error_Code != 0){			
				throw new Exception("终端管理服务，报废终端终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			reponse = new FlexReponse(request,0,"报废"+list.size()+"个终端成功！");
		}
		return reponse;
	}
	
	private FlexReponse query00ALeadData(FlexRequest request) throws Exception{
		T_TERMINAL_INFO terminal = (T_TERMINAL_INFO) request.getBean("terminal");
		DBBuilder builder = new DBBuilder();
		String sql = "select IMEI_NUMBER,UIM_NUMBER,MSISDN,HOTEL_ID,PRODUCT_ORG_ID,TERMINAL_TYPE,STATUS ";
		sql += " from T_TERMINAL_INFO ";
		
		boolean haswhere = false;
		if(terminal.IMEI_NUMBER != null && terminal.IMEI_NUMBER.trim().length()>0){
			sql += "  where IMEI_NUMBER = '"+terminal.IMEI_NUMBER+"'";
			haswhere = true;
		}
		if(terminal.UIM_NUMBER != null && terminal.UIM_NUMBER.trim().length()>0){
			if(haswhere){
				sql += "  and UIM_NUMBER = '"+terminal.UIM_NUMBER+"'";
			}else{
				sql += "  where UIM_NUMBER = '"+terminal.UIM_NUMBER+"'";
				haswhere = true;
			}
			
		}
		if(terminal.MSISDN != null && terminal.MSISDN.trim().length()>0){
			if(haswhere){
				sql += "  and MSISDN = '"+terminal.MSISDN+"'";
			}else{
				sql += "  where MSISDN = '"+terminal.MSISDN+"'";
				haswhere = true;
			}
			
		}	
		if(terminal.HOTEL_ID != null && terminal.HOTEL_ID.trim().length()>0){
			if(haswhere){
				sql += "  and HOTEL_ID = '"+terminal.HOTEL_ID+"'";
			}else{
				sql += "  where HOTEL_ID = '"+terminal.HOTEL_ID+"'";
				haswhere = true;
			}
			
		}	
		if(terminal.PRODUCT_ORG_ID != null && terminal.PRODUCT_ORG_ID.trim().length()>0){
			if(haswhere){
				sql += "  and PRODUCT_ORG_ID = '"+terminal.PRODUCT_ORG_ID+"'";
			}else{
				sql += "  where PRODUCT_ORG_ID = '"+terminal.PRODUCT_ORG_ID+"'";
				haswhere = true;
			}
			
		}	
		if(terminal.TERMINAL_TYPE != null && terminal.TERMINAL_TYPE.trim().length()>0){
			if(haswhere){
				sql += "  and TERMINAL_TYPE = '"+terminal.TERMINAL_TYPE+"'";
			}else{
				sql += "  where TERMINAL_TYPE = '"+terminal.TERMINAL_TYPE+"'";
				haswhere = true;
			}
			
		}	
		if(terminal.STATUS != null && terminal.STATUS.trim().length()>0){
			if(haswhere){
				sql += "  and STATUS = "+terminal.STATUS+"";
			}else{
				sql += "  where STATUS = "+terminal.STATUS+"";
				haswhere = true;
			}
			
		}
		sql += " order by Join_Time";
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){		
			throw new Exception("终端管理服务，获取终端导出数据出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse lead00AData(FlexRequest request) throws Exception{
		String xml = (String) request.getBean("xml");		
		DBBuilder builder = new DBBuilder();
		String url = builder.leadDataToFile(xml, "TerminalData");
		if(url == null){
			throw new Exception("制作导出数据Excel文件出错！");
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("url", url);
		return reponse;
	}
	
	private FlexReponse initQueryHotelTerminalPageParm(FlexRequest request) throws Exception{
		Map<String,Object> map = new HashMap<String,Object>();
		map.put("label", "全部");
		map.put("data", "");
		ParamHandler handler = new ParamHandler();
		List<Map<String,Object>> list_factory_edit = handler.getT_Org_Info_Org_ID_Org_Name("('1')","('0')");
		List<Map<String,Object>> list_factory_query = handler.getT_Org_Info_Org_ID_Org_Name("('1')","('0')");
		List<Map<String,Object>> list_hotel_query = handler.getT_Org_Info_Org_ID_Org_Name("('3')","('0')");
		List<Map<String,Object>> list_hotel_edit = handler.getT_Org_Info_Org_ID_Org_Name("('3')","('0')");
		List<Map<String,Object>> list_terminal_status_edit = handler.getT_Terminal_Info_Status(null,"('1')");
		List<Map<String,Object>> list_terminal_status_query = handler.getT_Terminal_Info_Status(null,"('1')");
		List<Map<String,Object>> list_terminal_type_edit = handler.getT_Terminal_Type();
		List<Map<String,Object>> list_terminal_type_query = handler.getT_Terminal_Type();
		list_terminal_type_query.add(0, map);
		list_factory_query.add(0, map);
		list_hotel_query.add(0, map);
		list_terminal_status_query.add(0, map);
		list_terminal_type_query.add(0, map);
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list_factory_edit", list_factory_edit);
		reponse.set("list_factory_query", list_factory_query);
		reponse.set("list_hotel_query", list_hotel_query);
		reponse.set("list_hotel_edit", list_hotel_edit);
		reponse.set("list_terminal_status_edit", list_terminal_status_edit);
		reponse.set("list_terminal_status_query", list_terminal_status_query);
		reponse.set("list_terminal_type_edit", list_terminal_type_edit);
		reponse.set("list_terminal_type_query", list_terminal_type_query);
		return reponse;
	}
	
	private FlexReponse queryPageHotelTerminal(FlexRequest request) throws Exception{
		T_TERMINAL_INFO terminal = (T_TERMINAL_INFO) request.getBean("terminal");
		String CURRENT_PAGE_NO = (String) request.getBean("CURRENT_PAGE_NO");
		
		DBBuilder builder = new DBBuilder();			
		
		String field = "Imei_Number,Uim_Number,Msisdn,Status,Last_Hire_Time,Last_Quit_Hire_Time,Hire_Request_Time,Current_Hire_Time,Product_Org_ID,Terminal_Type,Product_Date,Maintain_Limited_Date,Join_Time,Join_User_ID,Distribute_Time,Distribute_User_ID,Hotel_ID,Hotel_Receiver,Content_Version,Room_No,Language,Last_HB_Time";
		String table = " T_TERMINAL_INFO ";
		String solution = "";
		boolean haswhere = false;
		if(terminal.HOTEL_ID != null && terminal.HOTEL_ID.trim().length()>0){
			solution += "  where Hotel_ID = '"+terminal.HOTEL_ID+"'";
			haswhere = true;
		}
		if(terminal.MSISDN != null && terminal.MSISDN.trim().length()>0){
			if(haswhere){
				solution += "  and MSISDN = '"+terminal.MSISDN+"'";
			}else{
				solution += "  where MSISDN = '"+terminal.MSISDN+"'";
				haswhere = true;
			}
			
		}
		if(terminal.STATUS != null && terminal.STATUS.trim().length()>0){
			if(haswhere){
				solution += "  and STATUS = "+terminal.STATUS+"";
			}else{
				solution += "  where STATUS = "+terminal.STATUS+"";
				haswhere = true;
			}
			
		}	
		
		if(terminal.ROOM_NO != null && terminal.ROOM_NO.trim().length()>0){
			if(haswhere){
				solution += "  and ROOM_NO = "+terminal.ROOM_NO+"";
			}else{
				solution += "  where ROOM_NO = "+terminal.ROOM_NO+"";
				haswhere = true;
			}
			
		}	
		solution += " order by Join_Time";
		Map<String,Object> map = builder.queryPage(field, table, solution, Integer.parseInt(CURRENT_PAGE_NO), false);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("终端管理服务，分页查询终端信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("map", map);
		return reponse;
	}
	
	private FlexReponse readyOpenTerminal(FlexRequest request) throws Exception{
		T_TERMINAL_INFO terminal = (T_TERMINAL_INFO) request.getBean("terminal");
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		int count = factory.getCount("T_TERMINAL_INFO", "IMEI_NUMBER='"+terminal.IMEI_NUMBER+"' and UIM_NUMBER='"+terminal.UIM_NUMBER+"' and Status = 10");
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("终端管理服务，预开通终端，判断当前终端是否符合条件出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		if(count == 0){
			factory.interruptTransaction();
			throw new Exception("终端管理服务，当前终端已经不存在，或没有处于未开通状态！");
		}
		
		String sql_update = "update T_Terminal_Info set Status = 11,Hire_Request_Time = sysdate ";
		sql_update += "where IMEI_NUMBER='"+terminal.IMEI_NUMBER+"' and UIM_NUMBER='"+terminal.UIM_NUMBER+"' and Status = 10";
		int count_readyopen = factory.excute(sql_update);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("终端管理服务，设置当前终端为预开通状态出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}else{
			if(count_readyopen != 1){
				factory.interruptTransaction();
				throw new Exception("终端管理服务，预开通终端个数超出限制！");
			}
		}
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("终端管理服务，提交终端预开通事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"预开通成功！");
		return reponse;
	}
	
	private FlexReponse cancleReadyOpenTerminal(FlexRequest request) throws Exception{
		T_TERMINAL_INFO terminal = (T_TERMINAL_INFO) request.getBean("terminal");
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		int count = factory.getCount("T_TERMINAL_INFO", "IMEI_NUMBER='"+terminal.IMEI_NUMBER+"' and UIM_NUMBER='"+terminal.UIM_NUMBER+"' and Status = 11");
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("终端管理服务，取消预开通终端，判断当前终端是否符合条件出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		if(count == 0){
			factory.interruptTransaction();
			throw new Exception("终端管理服务，当前终端已经不存在，或没有处于预开通状态！");
		}
		
		String sql_update = "update T_Terminal_Info set Status = 10,Hire_Request_Time = sysdate ";
		sql_update += "where IMEI_NUMBER='"+terminal.IMEI_NUMBER+"' and UIM_NUMBER='"+terminal.UIM_NUMBER+"' and Status = 11";
		int count_releaseOpen = factory.excute(sql_update);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("终端管理服务，设置当前终端为未开通状态出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}else{
			if(count_releaseOpen != 1){
				factory.interruptTransaction();
				throw new Exception("终端管理服务，取消预开通终端个数（"+count_releaseOpen+"）超出限制！");
			}
		}
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("终端管理服务，提交终端取消预开通事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"取消预开通成功！");
		return reponse;
	}
	
	private FlexReponse backHireTerminal(FlexRequest request) throws Exception{
		T_TERMINAL_INFO terminal = (T_TERMINAL_INFO) request.getBean("terminal");
		String reason = (String) request.getBean("reason");
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		Map<String,Object> map_terminal = factory.queryMap("select Status from T_TERMINAL_INFO where IMEI_NUMBER='"+terminal.IMEI_NUMBER+"' and UIM_NUMBER='"+terminal.UIM_NUMBER+"'");
		//int count = factory.getCount("T_TERMINAL_INFO", "IMEI_NUMBER='"+terminal.IMEI_NUMBER+"' and UIM_NUMBER='"+terminal.UIM_NUMBER+"' and Status = 12 ");
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("终端管理服务，退租终端，判断当前终端是否符合条件出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(map_terminal == null){
			factory.interruptTransaction();
			throw new Exception("终端管理服务，当前终端已经不存在！");
		}
		String status_terminal = (String) map_terminal.get("STATUS");
		if(!status_terminal.equals("12")){
			factory.interruptTransaction();
			throw new Exception("终端管理服务，当前终端没有处于开通状态！");
		}
		
		String sql_update = "update T_Terminal_Info set Status=10, Hire_Request_Time=null,Last_Hire_Time=to_date('"+terminal.CURRENT_HIRE_TIME+"','yyyy-mm-dd hh24:mi:ss'), Last_Quit_Hire_Time = sysdate ";
		sql_update += "where IMEI_NUMBER='"+terminal.IMEI_NUMBER+"' and UIM_NUMBER='"+terminal.UIM_NUMBER+"' and Status = 12 ";
		int count_cancleHire = factory.excute(sql_update);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("终端管理服务，设置当前终端为未开通状态出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}else{
			if(count_cancleHire != 1){
				factory.interruptTransaction();
				throw new Exception("终端管理服务，退租终端个数（"+count_cancleHire+"）超出限制！");
			}
		}
		
		DateUtil du = new DateUtil();
		String sdrTableName = "T_SDR_" + du.getStringTime("yyyyMM");
		String sql_update_sdr = "update "+sdrTableName+" set ";
		sql_update_sdr += " End_Time=sysdate,";
		sql_update_sdr += " Quit_Src=1,";
		sql_update_sdr += " Status=1,";
		sql_update_sdr += " Quit_Notes='"+reason+"' ";
		sql_update_sdr += " Where Imei_Number='"+terminal.IMEI_NUMBER+"' and Uim_Number='"+terminal.UIM_NUMBER+"' and MSISDN='"+terminal.MSISDN+"' and status=0";
		int count_update_sdr = factory.excute(sql_update_sdr);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("终端管理服务，设置当前终端为未开通状态出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}else{
			if(count_update_sdr != 1){
				factory.interruptTransaction();
				throw new Exception("终端管理服务，退租终端，交换表（"+sdrTableName+"）数据更新个数（"+count_update_sdr+"）超出限制！");				
			}
		}
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("终端管理服务，提交终端退租事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"退租成功！");
		return reponse;
	}
	
	private FlexReponse doSDRTerminalConfirmInfo(FlexRequest request) throws Exception{
		T_SDR sdr = (T_SDR) request.getBean("sdr");
		String date_str = sdr.START_TIME;
		String[] elem = date_str.split(" ");
		String[] item = elem[0].split("-");
		String ViewTableName = "T_SDR_"+item[0]+item[1];
		DBBuilder builder = new DBBuilder();
		String sql = "update "+ViewTableName+" set Hire_Confirm_Operator = '"+request.user_id+"', Hire_Confirm_Time = sysdate where ID = "+sdr.ID;
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("终端管理服务，确认终端运行状态出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"设置确认信息成功！");
		return reponse;
	}
}
