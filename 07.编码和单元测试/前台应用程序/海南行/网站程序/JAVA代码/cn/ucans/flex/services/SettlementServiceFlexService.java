package cn.ucans.flex.services;

import java.util.HashMap;
import java.util.List;
import java.util.Map;


import chobits.common.DateUtil;
import chobits.db.DBBuilder;
import chobits.db.DBFactory;
import chobits.log.Logger;
import cn.ucans.bean.T_BILL_DETAIL;
import cn.ucans.bean.T_SERVICE_CASH;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class SettlementServiceFlexService extends FlexService{
	
	private Logger logger = new Logger(SettlementServiceFlexService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------账务管理任务 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 1300:
					reponse = this.queryMSISDNFromRoomNo(request);
					break;
				case 1301:
					reponse = this.queryWaitSettleCash(request);
					break;
				case 1302:
					reponse = this.doBackHireAndSettlement(request);
					break;
				case 1303:
					reponse = this.cancellingAccount(request);
					break;
				case 1304:
					reponse = this.queryBillInfo(request);
					break;
				case 1305:
					reponse = this.leadData(request);
					break;
				case 1306:
					reponse = this.statisticsData(request);
					break;
				case 1307:
					reponse = this.queryCanFreeSettleCash(request);
					break;
				case 1308:
					reponse = this.doFreeCashSettlement(request);
					break;
				case 1309:
					reponse = this.repeatPrintData(request);
					break;
				case 1310:
					reponse = this.queryPutDownSettleCash(request);
					break;
				case 1311:
					reponse = this.putAccount(request);
					break;
				case 1312:
					reponse = this.queryAllSettlement(request);
					break;
				case 1313:
					reponse = this.downAccount(request);
					break;
				case 1314:
					reponse = this.findMaxAndMinTimeForBill(request);
					break;
				case 1315:
					reponse = this.doFreeCashSettlementForHotel(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------账务管理任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------账务管理任务 执行失败-------------------------------");
		}		
		return reponse;
	}
	
	private FlexReponse putAccount(FlexRequest request) throws Exception{
		T_SERVICE_CASH cash = (T_SERVICE_CASH) request.getBean("cash");
		
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		if(cash.CREATE_TIME.trim().length()==0){
			factory.interruptTransaction();
			throw new Exception("该账单（序号："+cash.SERVICE_ID+"）创建时间为空，不能进行挂账或解挂操作！");
		}
		
		String time_format = cash.CREATE_TIME;
		String[] elem1 = time_format.split(" ");
		String[] elem2 = elem1[0].split("-");
		String serviceTableName = "T_Service_CASH_"+elem2[0]+elem2[1];
		String update_sql = "update "+serviceTableName+" set Status='"+cash.STATUS+"'";
		update_sql += " where Service_ID="+cash.SERVICE_ID+" and Status='0'";
		int update_count = factory.excute(update_sql);
		if(factory.SQL_Error_Code != 0){	
			throw new Exception("账务管理，挂账"+cash.SERVICE_ID+"出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(update_count != 1){
			factory.interruptTransaction();
			throw new Exception("账务管理，挂账所用账单"+cash.SERVICE_ID+"已经不存在，或已经被挂账！（原始改变记录数："+update_count+"）");
		}
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理，提交挂账"+cash.SERVICE_ID+"事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"挂账成功！");
		return reponse;
	}
	
	private FlexReponse downAccount(FlexRequest request) throws Exception{
		T_SERVICE_CASH cash = (T_SERVICE_CASH) request.getBean("cash");
		
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		if(cash.CREATE_TIME.trim().length()==0){
			factory.interruptTransaction();
			throw new Exception("该账单（序号："+cash.SERVICE_ID+"）创建时间为空，不能进行挂账或解挂操作！");
		}
		
		String time_format = cash.CREATE_TIME;
		String[] elem1 = time_format.split(" ");
		String[] elem2 = elem1[0].split("-");
		String serviceTableName = "T_Service_CASH_"+elem2[0]+elem2[1];
		String update_sql = "update "+serviceTableName+" set Status='"+cash.STATUS+"'";
		update_sql += " where Service_ID="+cash.SERVICE_ID+" and Status='2'";
		int update_count = factory.excute(update_sql);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，解挂账单"+cash.SERVICE_ID+"费用出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(update_count != 1){
			factory.interruptTransaction();
			throw new Exception("账务管理销账，即将解挂的账单"+cash.SERVICE_ID+"已经不存在，或已经被解挂！（原始改变记录数："+update_count+"）");
		}
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，提交解挂"+cash.SERVICE_ID+"事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"账单解挂成功！");
		return reponse;
	}
	
	private FlexReponse queryMSISDNFromRoomNo(FlexRequest request) throws Exception{
		T_SERVICE_CASH cash = (T_SERVICE_CASH) request.getBean("cash");
		ParamHandler handler = new ParamHandler();
		String room_no = null;
		if(cash.ROOM_NO != null && cash.ROOM_NO.trim().length()>0){
			room_no = cash.ROOM_NO;
		}
		List<Map<String,Object>> list = handler.getPhoneFromRoomNO(room_no,request.org_id);

		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse queryWaitSettleCash(FlexRequest request) throws Exception{
		T_SERVICE_CASH cash = (T_SERVICE_CASH) request.getBean("cash");
		
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		String sql_map = "select Imei_Number,Uim_Number,Current_Hire_Time,Status from T_Terminal_Info where Msisdn='"+cash.MSISDN+"'";
		Map<String,Object> map_last_hire_map = factory.queryMap(sql_map);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理结账，查询终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(map_last_hire_map == null){
			factory.interruptTransaction();
			throw new Exception("终端"+cash.MSISDN+"已经不存在！");			
		}
		String Imei_Number = (String) map_last_hire_map.get("IMEI_NUMBER");
		String Uim_Number = (String) map_last_hire_map.get("UIM_NUMBER");
		String Current_Hire_Time = (String) map_last_hire_map.get("CURRENT_HIRE_TIME");
		
		String sql_sdr = "select Imei_Number,Uim_Number,Msisdn,Room_NO,START_TIME,END_TIME,Status from V_SDR where Hotel_ID = '"+request.org_id+"' and Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Status in (0,1,2)"; 
		if(cash.ROOM_NO != null && cash.ROOM_NO.trim().length()>0){
			sql_sdr += " and ROOM_NO = '"+cash.ROOM_NO+"' ";
		}
		if(cash.MSISDN != null && cash.MSISDN.trim().length()>0){
			sql_sdr += " and MSISDN = '"+cash.MSISDN+"' ";
		}
		sql_sdr += " order by Start_Time ";
		
		List<Map<String,Object>> list_sdr = factory.queryList(sql_sdr); 
		if(factory.SQL_Error_Code != 0){
			throw new Exception("账务管理，查询待终端使用记录发生错误："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		int status_0 = 0;
		int status_1 = 0;
		int status_2 = 0;
		
		int list_sdr_size = list_sdr.size();
		
		if(list_sdr_size == 0){
			factory.interruptTransaction();
			throw new Exception("没有查询到当前终端的任何使用记录！");
		}
		
		for(int i=0;i<list_sdr.size();i++){
			Map<String,Object> map_sdr = list_sdr.get(i);
			String status_sdr = (String) map_sdr.get("STATUS");
			if(status_sdr.equals("0")){
				status_0 ++;
			}else if(status_sdr.equals("1")){
				status_1 ++;
			}else if(status_sdr.equals("2")){
				status_2 ++;
			}
		}
		
		int sign = 0;
		List<Map<String,Object>> list = null;
		int type = 0;
		
		String sql_query_cash_list = "select Service_ID,Msisdn,Cash,Real_Pay_Cash,Start_Time,End_Time,May_Free_Cash,Free_Cash,Status ";
		sql_query_cash_list += " from V_Service_Cash ";
		sql_query_cash_list += " where Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Msisdn='"+cash.MSISDN+"' and Status in ('0')";
		boolean do_lunxun = false;
		long seq_common = -1;
		if(status_0 == 0){
			if(status_1 == 0){
				if(status_2 == 0){
					factory.interruptTransaction();
					throw new Exception("没有查询到当前终端的任何使用记录！");
				}else{
					list = factory.queryList(sql_query_cash_list);
					if(factory.SQL_Error_Code != 0){		
						throw new Exception("账务管理结算，终端隐形退租成功，查询账单信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
					}
					if(list.size()<1){
//						throw new Exception("内部错误：账务管理结算时，虽然终端隐形退租成功，但查询账单信息出错，原因：在V_Service_Cash表中数据查询对应终端（"+Imei_Number+","+Uim_Number+"）账单个数为0 ！");
						throw new Exception("没有查询终端使用记录或账单数据 ！");
					}
					type = 2;
				}
			}else{
				if(status_2 == 0){
					seq_common = factory.getOracleSequreNextValue("S_Command");
					if(factory.SQL_Error_Code != 0){		
						factory.interruptTransaction();
						throw new Exception("账务管理结算，获取账单结算任务流水S_Command序列出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
					}
					
					String sql_commond = "insert Into T_Command (SEQ,Type,Imei_Number,Uim_Number,Status,Parm1,Create_Time) values ";
					sql_commond += " ("+seq_common+",'0','"+Imei_Number+"',"+Uim_Number+",'0','"+Current_Hire_Time+"',sysdate )";
					factory.excute(sql_commond);
					if(factory.SQL_Error_Code != 0){		
						throw new Exception("账务管理结算，提交终端退租事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
					}					
					do_lunxun = true;
					type = 2;
				}else{
					seq_common = factory.getOracleSequreNextValue("S_Command");
					if(factory.SQL_Error_Code != 0){		
						factory.interruptTransaction();
						throw new Exception("账务管理结算，获取账单结算任务流水S_Command序列出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
					}
					
					String sql_commond = "insert Into T_Command (SEQ,Type,Imei_Number,Uim_Number,Status,Parm1,Create_Time) values ";
					sql_commond += " ("+seq_common+",'0','"+Imei_Number+"',"+Uim_Number+",'0','"+Current_Hire_Time+"',sysdate )";
					factory.excute(sql_commond);
					if(factory.SQL_Error_Code != 0){		
						throw new Exception("账务管理结算，提交终端退租事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
					}					
					do_lunxun = true;
					type = 2;
				}
			}
		}else if(status_0 == 1){
			sql_sdr = "select Imei_Number,Uim_Number,Msisdn,Room_NO,START_TIME,END_TIME,Status from V_SDR where Hotel_ID = '"+request.org_id+"' and Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Status in (0)"; 
			if(cash.ROOM_NO != null && cash.ROOM_NO.trim().length()>0){
				sql_sdr += " and ROOM_NO = '"+cash.ROOM_NO+"' ";
			}
			if(cash.MSISDN != null && cash.MSISDN.trim().length()>0){
				sql_sdr += " and MSISDN = '"+cash.MSISDN+"' ";
			}
			sql_sdr += " order by Start_Time ";
			
			list_sdr = factory.queryList(sql_sdr); 
			if(factory.SQL_Error_Code != 0){
				throw new Exception("账务管理，查询待终端使用记录发生错误："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			list = list_sdr;
		}else if(status_0 > 1){
			factory.interruptTransaction();
			throw new Exception("内部错误，SDR表中该终端状态为0的记录数超过1个！");
		}
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){
			throw new Exception("账务管理，提交查询待终端使用记录事务发生错误："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		DBBuilder builder = new DBBuilder();
		String common_status = "2";
		String common_message = "";
		int return_sign = 0;
		
		if(do_lunxun){			
			for(int i=1;i<=240;i++){ //原始值240
				String sql_common_query = "select Status,Return_Info from T_Command where SEQ="+seq_common;
				Map<String,Object> map_commond = builder.queryMap(sql_common_query);
				if(builder.SQL_Error_Code != 0){		
					throw new Exception("账务管理结算，退租终端，轮询T_Commond数据（序列："+seq_common+"）出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
				}
				boolean ok = false;
				if(map_commond != null){
					common_status = (String) map_commond.get("STATUS");
					common_message = (String) map_commond.get("RETURN_INFO");
					if(common_status.equals("2") || common_status.equals("3")){
						ok = true;
					}
				}else{
					throw new Exception("账务管理结算，退租终端，轮询T_Command表数据出错，原因：该序列"+seq_common+"对应的数据已经不存在，或意外丢失！");
				}	
				if(ok){
					break;
				}else{
					Thread.sleep(1000);
				}
			}
			
			if(common_status.equals("2")){			
				list = builder.queryList(sql_query_cash_list);
				if(builder.SQL_Error_Code != 0){		
					throw new Exception("账务管理结算，终端隐形退租成功，查询账单信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
				}
				if(list.size()<1){
//					throw new Exception("内部错误：账务管理结算时，虽然终端隐形退租成功，但查询账单信息出错，原因：在V_Service_Cash表中数据查询对应终端（"+Imei_Number+","+Uim_Number+"）账单个数为0 ！");
					throw new Exception("没有查询到终端（"+Imei_Number+","+Uim_Number+","+cash.MSISDN+"）账单数据 ！");
				}
//				throw new Exception("账务管理结算，没有查询到用于结账的使用记录和账单数据！");
				return_sign = 0;
			}else if(common_status.equals("3")){
				if(common_message == null || common_message.trim().length()==0){
					common_message = "账务管理结算,结算失败，未知原因！";
				}
				return_sign = -1;
			}else{
				common_status = "-2000";
				common_message = "账务管理结算,结算超时（命令状态码："+common_status+"）";
				return_sign = -1;
			}
			
		}
		
		FlexReponse reponse = new FlexReponse(request,return_sign,common_message);
		reponse.set("list", list);
		reponse.set("type", type);
		return reponse;
	}
	
	private FlexReponse queryCanFreeSettleCash(FlexRequest request) throws Exception{
		T_SERVICE_CASH cash = (T_SERVICE_CASH) request.getBean("cash");
		
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		String sql_map = "select Imei_Number,Uim_Number,Current_Hire_Time,Status from T_Terminal_Info where Msisdn='"+cash.MSISDN+"'";
		Map<String,Object> map_last_hire_map = factory.queryMap(sql_map);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理结账，查询终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(map_last_hire_map == null){
			factory.interruptTransaction();
			throw new Exception("账务管理结账，您要查询的终端"+cash.MSISDN+"已经不存在！");			
		}
		String Imei_Number = (String) map_last_hire_map.get("IMEI_NUMBER");
		String Uim_Number = (String) map_last_hire_map.get("UIM_NUMBER");
		
		String sql = "select SERVICE_ID,Msisdn,Room_NO,CASH,REAL_PAY_CASH,START_TIME,END_TIME,MAY_FREE_CASH,Free_Cash ";
		sql += " from V_Service_Cash ";
		sql += " where Hotel_ID = '"+request.org_id+"' and Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Status='0' ";
		
		if(cash.MSISDN != null && cash.MSISDN.trim().length()>0){
			sql += " and MSISDN = '"+cash.MSISDN+"'";
		}
		sql += " order by Start_Time";
		
		List<Map<String,Object>> list = factory.queryList(sql);
		if(factory.SQL_Error_Code != 0){
			throw new Exception("账务管理，查询可以用于减免费用的账单信息发生错误："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){
			throw new Exception("账务管理，查询可以用于减免费用的账单信息发生错误："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse queryPutDownSettleCash(FlexRequest request) throws Exception{
		T_SERVICE_CASH cash = (T_SERVICE_CASH) request.getBean("cash");
		
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		String sql_map = "select Imei_Number,Uim_Number,Current_Hire_Time,Status from T_Terminal_Info where Msisdn='"+cash.MSISDN+"'";
		Map<String,Object> map_last_hire_map = factory.queryMap(sql_map);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理挂账、解挂，查询终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(map_last_hire_map == null){
			factory.interruptTransaction();
			throw new Exception("账务管理挂账、解挂，您要查询的终端"+cash.MSISDN+"已经不存在！");			
		}
		String Imei_Number = (String) map_last_hire_map.get("IMEI_NUMBER");
		String Uim_Number = (String) map_last_hire_map.get("UIM_NUMBER");
		
		String sql = "select SERVICE_ID,Msisdn,Room_NO,CASH,REAL_PAY_CASH,START_TIME,END_TIME,MAY_FREE_CASH,Free_Cash,Status,Create_Time ";
		sql += " from V_Service_Cash ";
		sql += " where Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Hotel_ID = '"+request.org_id+"' and Status in ('0','2') ";
		
		if(cash.MSISDN != null && cash.MSISDN.trim().length()>0){
			sql += " and MSISDN = '"+cash.MSISDN+"'";
		}
		sql += " order by Start_Time";
		
		List<Map<String,Object>> list = factory.queryList(sql);
		if(factory.SQL_Error_Code != 0){
			throw new Exception("账务管理，查询可以用于挂账解挂的账单信息发生错误："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){
			throw new Exception("账务管理，提交查询可以用于挂账解挂的账单信息事务错误："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse queryAllSettlement(FlexRequest request) throws Exception{
		T_SERVICE_CASH cash = (T_SERVICE_CASH) request.getBean("cash");
		String notbefore = (String) request.getBean("notbefore");
		String notafter = (String) request.getBean("notafter");
		DBBuilder builder = new DBBuilder();
		
		String sql = "";
		List<Map<String,Object>> list = null;
		if(request.type == 1){
			sql = "select Service_ID,Imei_Number,Uim_Number,Msisdn,Room_No,Hotel_ID,Broker_ID,Cash,Real_Pay_Cash,Start_Time,End_Time,Create_Time,May_Free_Cash,Free_Cash,Cash_Type,Status,Oper_User,Oper_Time,Advance_User,Pay_Cash_SEQ ";
			sql += " from V_Service_Cash ";
			sql += " where Hotel_ID = '"+request.org_id+"' and CREATE_TIME>=to_date('"+notbefore+"','yyyy-mm-dd hh24:mi:ss') and CREATE_TIME<=to_date('"+notafter+"','yyyy-mm-dd hh24:mi:ss') ";
			if(cash.MSISDN != null && cash.MSISDN.trim().length()>0){
				sql += " and MSISDN = '"+cash.MSISDN+"'";
			}
			if(cash.ROOM_NO != null && cash.ROOM_NO.trim().length()>0){
				sql += " and ROOM_NO = '"+cash.ROOM_NO+"'";
			}
			if(cash.STATUS != null && cash.STATUS.trim().length()>0){
				sql += " and STATUS = '"+cash.STATUS+"'";
			}
			list = builder.queryList(sql);
			if(builder.SQL_Error_Code != 0){
				throw new Exception("账务管理，查询账单信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}
		}else{
			sql = "select BILL_ID,Imei_Number,Uim_Number,Msisdn,Room_No,Hotel_ID,Broker_ID,Org_Rec_ID,Service_ID,Cash,Start_Time,End_Time,Create_Time,Free_Flag,Cash_Type,Status ";
			sql += " from V_Bill_Detail";
			sql += " where Hotel_ID = '"+request.org_id+"'  and CREATE_TIME>=to_date('"+notbefore+"','yyyy-mm-dd hh24:mi:ss') and CREATE_TIME<=to_date('"+notafter+"','yyyy-mm-dd hh24:mi:ss') ";
			if(cash.MSISDN != null && cash.MSISDN.trim().length()>0){
				sql += " and MSISDN = '"+cash.MSISDN+"'";
			}
			if(cash.ROOM_NO != null && cash.ROOM_NO.trim().length()>0){
				sql += " and ROOM_NO = '"+cash.ROOM_NO+"'";
			}
			if(cash.STATUS != null && cash.STATUS.trim().length()>0){
				sql += " and STATUS = '"+cash.STATUS+"'";
			}
			list = builder.queryList(sql);
			if(builder.SQL_Error_Code != 0){
				throw new Exception("账务管理，查询详单信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}
		}
		
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse repeatPrintData(FlexRequest request) throws Exception{
		
		T_SERVICE_CASH cash = (T_SERVICE_CASH) request.getBean("cash");
		
		String sql = "select Service_ID,Imei_Number,Uim_Number,Msisdn,Room_No,Hotel_ID,Broker_ID,Cash,Real_Pay_Cash,Start_Time,End_Time,Create_Time,May_Free_Cash,Free_Cash,Cash_Type,Status,Oper_User,Oper_Time,Advance_User,Pay_Cash_SEQ ";
		sql += " from V_Service_Cash ";
		sql += " where Pay_Cash_SEQ='"+cash.PAY_CASH_SEQ+"' order by Start_Time";
		
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("账务管理，查询待结算的账单信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		String sql_user = "select User_Name from T_User_Info where User_ID='"+request.user_id+"'";
		Map<String,Object> map_user = builder.queryMap(sql_user);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("平台报表、查询操作员姓名，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(map_user == null){
				throw new Exception("获取操作员姓名失败，找不到编号为"+request.user_id+"的操作员信息！");
			}
		}
		String UserName = (String) map_user.get("USER_NAME");
		String sql_org = "select Org_Name from T_Org_Info where Org_ID='"+request.org_id+"'";
		Map<String,Object> map_org = builder.queryMap(sql_org);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("平台报表、查询操作员姓名，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(map_user == null){
				throw new Exception("获取操作员所在组织名称，找不到编号为"+request.org_id+"的组织信息！");
			}
		}
		String OrgName = (String) map_org.get("ORG_NAME");
		
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		reponse.set("OrgName", UserName);
		reponse.set("UserName", OrgName);
		return reponse;
	}
	
	private FlexReponse findMaxAndMinTimeForBill(FlexRequest request) throws Exception{
		T_SERVICE_CASH cash = (T_SERVICE_CASH) request.getBean("cash");
		
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		String sql = "select Imei_Number,Uim_Number,Current_Hire_Time,Status from T_Terminal_Info where Msisdn='"+cash.MSISDN+"'";
		Map<String,Object> map_last_hire_map = factory.queryMap(sql);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理查询详单，查询隐形退租终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(map_last_hire_map == null){
			factory.interruptTransaction();
			throw new Exception("账务管理查询详单，您要结账的终端"+cash.MSISDN+"已经不存在！");			
		}
		
		String Imei_Number = (String) map_last_hire_map.get("IMEI_NUMBER");
		String Uim_Number = (String) map_last_hire_map.get("UIM_NUMBER");
		
		int count = factory.getCount("V_Bill_Detail", "Hotel_ID = '"+request.org_id+"' and Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"'");
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理查询详单，查询符合条件的详单个数出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(count == 0){
			factory.interruptTransaction();
			throw new Exception("没有找到终端（"+Imei_Number+","+Uim_Number+","+cash.MSISDN+"）的任何详单数据！");
		}
		
		
		String sql_find = "select min(Create_Time) as MIN_TIME,max(Create_Time) as MAX_TIME from V_Bill_Detail ";
		sql_find += " where Hotel_ID = '"+request.org_id+"' and Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"'";
		
		Map<String,Object> map_time_map = factory.queryMap(sql_find);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理查询详单，查询详单中最大最小时间出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(map_time_map == null){
			factory.interruptTransaction();
			throw new Exception("账务管理查询详单，您要结账的终端（"+Imei_Number+","+Uim_Number+"）已生成状态的详单个数为0！");			
		}
		
		String MIN_TIME = (String) map_time_map.get("MIN_TIME");
		String MAX_TIME = (String) map_time_map.get("MAX_TIME");
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理查询详单，提交查询最大日期和最小日期事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("MAX_TIME", MAX_TIME);
		reponse.set("MIN_TIME", MIN_TIME);
		return reponse;
	}
	
	private FlexReponse doBackHireAndSettlement(FlexRequest request) throws Exception{
		T_SERVICE_CASH cash = (T_SERVICE_CASH) request.getBean("cash");
		DBBuilder builder = new DBBuilder();
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		String sql = "select Imei_Number,Uim_Number,Current_Hire_Time,Status from T_Terminal_Info where Msisdn='"+cash.MSISDN+"'";
		Map<String,Object> map_last_hire_map = factory.queryMap(sql);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理结账，查询隐形退租终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(map_last_hire_map == null){
			factory.interruptTransaction();
			throw new Exception("账务管理结账，您要结账的终端"+cash.MSISDN+"已经不存在！");			
		}
		String Imei_Number = (String) map_last_hire_map.get("IMEI_NUMBER");
		String Uim_Number = (String) map_last_hire_map.get("UIM_NUMBER");
		String Current_Hire_Time = (String) map_last_hire_map.get("CURRENT_HIRE_TIME");
		String Status = (String) map_last_hire_map.get("STATUS");
		
		String sql_sdr = "select Status from V_SDR where Hotel_ID = '"+request.org_id+"' and Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Status in (0,1,2)"; 
		if(cash.ROOM_NO != null && cash.ROOM_NO.trim().length()>0){
			sql_sdr += " and ROOM_NO = '"+cash.ROOM_NO+"' ";
		}
		if(cash.MSISDN != null && cash.MSISDN.trim().length()>0){
			sql_sdr += " and MSISDN = '"+cash.MSISDN+"' ";
		}
		sql_sdr += " order by Start_Time ";
		
		List<Map<String,Object>> list_sdr = factory.queryList(sql_sdr); 
		if(factory.SQL_Error_Code != 0){
			throw new Exception("账务管理，隐形结账，查询待终端使用记录发生错误："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		int status_0 = 0;
		int status_1 = 0;
		int status_2 = 0;
		
		int list_sdr_size = list_sdr.size();
		
		if(list_sdr_size == 0){
			factory.interruptTransaction();
			throw new Exception("隐形结账，没有查询到当前终端的任何使用记录（SDR表合法记录个数为0）！");
		}
		
		for(int i=0;i<list_sdr.size();i++){
			Map<String,Object> map_sdr = list_sdr.get(i);
			String status_sdr = (String) map_sdr.get("STATUS");
			if(status_sdr.equals("0")){
				status_0 ++;
			}else if(status_sdr.equals("1")){
				status_1 ++;
			}else if(status_sdr.equals("2")){
				status_2 ++;
			}
		}
		
		boolean dook_back_hire = false;	
		boolean do_commond = false;
		
		if(status_0 == 0){
			if(status_1 == 0){
				if(status_2 == 0){
					factory.interruptTransaction();
					throw new Exception("没有查询到当前终端的任何使用记录！");
				}else{
					dook_back_hire = false;
					do_commond = false;
				}
			}else{
				if(status_2 == 0){
					dook_back_hire = false;
					do_commond = true;
				}else{
					dook_back_hire = false;
					do_commond = false;
				}
			}
		}else if(status_0 == 1){
			dook_back_hire = true;
			do_commond = true;
		}else if(status_0 > 1){
			factory.interruptTransaction();
			throw new Exception("内部错误，SDR表中该终端状态为0的记录数超过1个！");
		}
		DateUtil du = new DateUtil();
		String sdrTableName = "T_SDR_" + du.getStringTime("yyyyMM");
		
		if(dook_back_hire){
			if(Status.equals("12")){
				String sql_update = "update T_Terminal_Info set Status=10, Hire_Request_Time=null,Last_Hire_Time=to_date('"+Current_Hire_Time+"','yyyy-mm-dd hh24:mi:ss'), Last_Quit_Hire_Time = sysdate ";
				sql_update += "where Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Msisdn='"+cash.MSISDN+"' and Status = 12 ";
				int count_cancleHire = factory.excute(sql_update);
				if(factory.SQL_Error_Code != 0){		
					throw new Exception("账务管理结算，设置当前终端为未开通状态出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}else{
					if(count_cancleHire != 1){
						factory.interruptTransaction();
						throw new Exception("账务管理结算，退租终端个数（"+count_cancleHire+"）超出限制！");
					}
				}
				
				String sql_update_sdr = "update "+sdrTableName+" set ";
				sql_update_sdr += " End_Time=sysdate,";
				sql_update_sdr += " Quit_Src=1,";
				sql_update_sdr += " Status=1,";
				sql_update_sdr += " Quit_Notes='账务结算，隐形退租' ";
				sql_update_sdr += " Where Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Msisdn='"+cash.MSISDN+"' and status=0";
				int count_update_sdr = factory.excute(sql_update_sdr);
				if(factory.SQL_Error_Code != 0){		
					throw new Exception("账务管理结算，设置当前终端为未开通状态出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}else{
					if(count_update_sdr != 1){
						factory.interruptTransaction();
						throw new Exception("账务管理结算，退租终端，交换表（"+sdrTableName+"）数据更新个数（"+count_update_sdr+"）超出限制！");				
					}
					
				}
				
			}
		}
		
		
		boolean do_lunxun = false;
		long seq_common = -1;
		
		if(do_commond){
			seq_common = factory.getOracleSequreNextValue("S_Command");
			if(factory.SQL_Error_Code != 0){		
				factory.interruptTransaction();
				throw new Exception("账务管理结算，获取账单结算任务流水S_Command序列出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			
			String sql_commond = "insert Into T_Command (SEQ,Type,Imei_Number,Uim_Number,Status,Parm1,Create_Time) values ";
			sql_commond += " ("+seq_common+",'0','"+Imei_Number+"',"+Uim_Number+",'0','"+Current_Hire_Time+"',sysdate )";
			factory.excute(sql_commond);
			if(factory.SQL_Error_Code != 0){		
				throw new Exception("账务管理结算，提交终端退租事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			do_lunxun = true;
		}	
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理结算，提交终端退租事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		String common_status = "";
		String common_message = "";
		
		if(do_lunxun){
			for(int i=1;i<=240;i++){ //原始值240
				String sql_common_query = "select Status,Return_Info from T_Command where SEQ="+seq_common;
				Map<String,Object> map_commond = builder.queryMap(sql_common_query);
				if(builder.SQL_Error_Code != 0){		
					throw new Exception("账务管理结算，退租终端，轮询T_Commond表数据（序列："+seq_common+"）出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
				}
				boolean ok = false;
				if(map_commond != null){
					common_status = (String) map_commond.get("STATUS");
					common_message = (String) map_commond.get("RETURN_INFO");
					if(common_status.equals("2") || common_status.equals("3")){
						ok = true;
					}
				}else{
					throw new Exception("账务管理结算，退租终端，轮询T_Command表数据出错，原因：该序列"+seq_common+"对应的数据已经不存在，或意外丢失！");
				}	
				if(ok){
					break;
				}else{
					Thread.sleep(1000);
				}
			}
		}else{
			common_status = "2";
		}

		List<Map<String,Object>> list_cash = null;
		int return_sign = 0;
		if(common_status.equals("2")){			
			String sql_query_cash_list = "select Service_ID,Msisdn,Cash,Real_Pay_Cash,Start_Time,End_Time,May_Free_Cash,Free_Cash,Status ";
			sql_query_cash_list += " from V_Service_Cash ";
			sql_query_cash_list += " where Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Msisdn='"+cash.MSISDN+"' and Status in ('0')";
			list_cash = builder.queryList(sql_query_cash_list);
			if(builder.SQL_Error_Code != 0){		
				throw new Exception("账务管理结算，终端隐形退租成功，查询账单信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}
			if(list_cash.size()<1){
//				throw new Exception("内部错误：账务管理结算时，虽然终端隐形退租成功，但查询账单信息出错，原因：在V_Service_Cash表中数据查询对应终端（"+Imei_Number+","+Uim_Number+"）账单个数为0 ！");
				throw new Exception("没有查询到终端（"+Imei_Number+","+Uim_Number+","+cash.MSISDN+"）账单数据 ！");
			}
//			throw new Exception("账务管理结算，没有查询到用于结账的使用记录和账单数据！");
			return_sign = 0;
		}else if(common_status.equals("3")){
			if(common_message == null || common_message.trim().length()==0){
				common_message = "账务管理结算,结算失败，未知原因！";
			}
			return_sign = -1;
		}else{
			common_status = "-2000";
			common_message = "账务管理结算,结算超时（命令状态码："+common_status+"）";
			return_sign = -1;
		}
		FlexReponse reponse = new FlexReponse(request,return_sign,common_message);
		reponse.set("list", list_cash);
		return reponse;
	}
	
	private FlexReponse doFreeCashSettlement(FlexRequest request) throws Exception{
		T_SERVICE_CASH cash = (T_SERVICE_CASH) request.getBean("cash");
		
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		DateUtil du = new DateUtil();
		String serviceTableName = "T_Service_Cash_" + du.getStringTime("yyyyMM");
		
		Map<String,Object> map_cash = factory.queryMap("select Cash,Real_Pay_Cash,May_Free_Cash,Free_Cash from V_Service_Cash where Service_ID="+cash.SERVICE_ID+" and Status='0'");
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，查找将要减免费用的账单"+cash.SERVICE_ID+"出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(map_cash == null){
			factory.interruptTransaction();
			throw new Exception("账务管理销账，将要减免费用的账单"+cash.SERVICE_ID+"已经不存在！");
		}
		
		int cash_int = Integer.parseInt((String) map_cash.get("CASH"));
		int may_cash_int = Integer.parseInt((String) map_cash.get("MAY_FREE_CASH"));
		int temp_free_cash_int = Integer.parseInt(cash.FREE_CASH);
		int real_pay_cash = cash_int - temp_free_cash_int;
		if(temp_free_cash_int > may_cash_int){
			factory.interruptTransaction();
			throw new Exception("账务管理销账，减免金额不能大于系统提供的可减免金额！");
		}
		if(real_pay_cash < 0){
			factory.interruptTransaction();
			throw new Exception("账务管理销账，减免金额不能大于预算总额！");
		}
		
//		String update_sql = "update "+serviceTableName+" set Real_Pay_Cash = "+real_pay_cash+" ,Free_Cash="+temp_free_cash_int+" ,Advance_User='"+request.user_id+"'";
		String update_sql = "update "+serviceTableName+" set Free_Cash="+temp_free_cash_int+" ,Advance_User='"+request.user_id+"'";
		update_sql += " where Service_ID="+cash.SERVICE_ID+" and Status=0 ";
		int update_count = factory.excute(update_sql);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，减免账单"+cash.SERVICE_ID+"费用出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(update_count != 1){
			factory.interruptTransaction();
			throw new Exception("账务管理销账，即将减免费用的账单"+cash.SERVICE_ID+"已经不存在，或已经被修改，请重新查询！（原始改变记录数："+update_count+"）");
		}
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，提交销账"+cash.SERVICE_ID+"事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"账单减免费用成功，可以继续结账操作！");
		return reponse;
	}
	
	private FlexReponse cancellingAccount(FlexRequest request) throws Exception{
		T_SERVICE_CASH cash = (T_SERVICE_CASH) request.getBean("cash");
		int type = request.type;
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		String sql_map = "select Imei_Number,Uim_Number,Current_Hire_Time,Status from T_Terminal_Info where Msisdn='"+cash.MSISDN+"'";
		Map<String,Object> map_last_hire_map = factory.queryMap(sql_map);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，查询终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(map_last_hire_map == null){
			factory.interruptTransaction();
			throw new Exception("账务管理销账，您要查询的终端"+cash.MSISDN+"已经不存在！");			
		}
		String Imei_Number = (String) map_last_hire_map.get("IMEI_NUMBER");
		String Uim_Number = (String) map_last_hire_map.get("UIM_NUMBER");
		
		long pay_seq = factory.getOracleSequreNextValue("Bill_SEQ");
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，查看销账流水Bill_SEQ序列出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		if(type == 1){
			List<Map<String,Object>> list_cash = factory.queryList("select Service_ID,Cash,Real_Pay_Cash,May_Free_Cash,Free_Cash from V_Service_Cash where Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Status='0' and (Real_Pay_Cash is null or Real_Pay_Cash=0)");
			if(factory.SQL_Error_Code != 0){		
				throw new Exception("账务管理销账，查找账单"+cash.SERVICE_ID+"数据出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			
			if(list_cash.size()==0){
				factory.interruptTransaction();
				throw new Exception("账务管理销账，将要销账的账单已经不存在！");
			}
			
			for(int i=0;i<list_cash.size();i++){
				Map<String,Object> map_cash = list_cash.get(i);
				String Service_ID_item = (String) map_cash.get("SERVICE_ID");
				String Cash = (String) map_cash.get("CASH");
				String Real_Pay_Cash_str = (String) map_cash.get("REAL_PAY_CASH");
				String Free_Cash_str = (String) map_cash.get("FREE_CASH");
				if(Cash.equals("") || Cash.trim().length()==0){
					factory.interruptTransaction();
					throw new Exception("账务管理销账，该账单"+cash.SERVICE_ID+"尚未生成，初始化金额CASH数值为空！");
				}
				if(!Real_Pay_Cash_str.equals("") || Real_Pay_Cash_str.trim().length()>0){
					factory.interruptTransaction();
					throw new Exception("账务管理销账，该账单"+cash.SERVICE_ID+"不存在，该账单可能已经被销账！");
				}
				
				int real_pay_cash_int = 0;
				int cash_int = Integer.parseInt(Cash);
				if(Free_Cash_str.equals("0") || Free_Cash_str.equals("") || Free_Cash_str.trim().length()==0){
					real_pay_cash_int = cash_int;
				}else{
					int free_cash_int = Integer.parseInt(Free_Cash_str);
					real_pay_cash_int = cash_int - free_cash_int;
					if(real_pay_cash_int <= 0){
						factory.interruptTransaction();
						throw new Exception("账务管理销账，账单"+cash.SERVICE_ID+"，预期交款总额："+cash_int+"，实际减免金额："+free_cash_int+"非法，预期赋予“实际交付钱数”不大于0，不合账务逻辑！");
					}
				}

				DateUtil du = new DateUtil();
				String serviceTableName = "T_Service_Cash_" + du.getStringTime("yyyyMM");
				
				String sql_cancle = "update "+serviceTableName+" set ";
				sql_cancle += " Real_Pay_Cash="+real_pay_cash_int+", Status = '1',Pay_Cash_SEQ="+pay_seq+",Oper_User='"+request.user_id+"',Oper_Time=sysdate";
				sql_cancle += " where Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Service_ID="+Service_ID_item+" and Status=0";
				int update_count = factory.excute(sql_cancle);
				if(factory.SQL_Error_Code != 0){		
					throw new Exception("账务管理销账，改变终端（"+Imei_Number+","+Uim_Number+"）销账流水出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
				}
				if(update_count > 1){
					factory.interruptTransaction();
					throw new Exception("账务管理销账，改变终端（"+Imei_Number+","+Uim_Number+"）销账流水出错，改变个数（"+update_count+"）超出限制！");			
				}else if(update_count == 0){
					factory.interruptTransaction();
					throw new Exception("账务管理销账，没有查询到用于终端（"+Imei_Number+","+Uim_Number+"）销账账单，该终端可能已经被销账！");			
				}
			}
			
		}else if(type == 2){
			Map<String,Object> map_cash = factory.queryMap("select Cash,Real_Pay_Cash,May_Free_Cash,Free_Cash from V_Service_Cash where Service_ID="+cash.SERVICE_ID+" and Status='0' and (May_Free_Cash is null or May_Free_Cash=0)");
			if(factory.SQL_Error_Code != 0){		
				throw new Exception("账务管理销账，查找账单"+cash.SERVICE_ID+"数据出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			
			if(map_cash == null){
				factory.interruptTransaction();
				throw new Exception("账务管理销账，将要销账的账单不存在，或已经被销账！");
			}
			
			String Cash = (String) map_cash.get("CASH");
			String Real_Pay_Cash_str = (String) map_cash.get("REAL_PAY_CASH");
			String Free_Cash_str = (String) map_cash.get("FREE_CASH");
			if(Cash.equals("") || Cash.trim().length()==0){
				factory.interruptTransaction();
				throw new Exception("账务管理销账，该账单"+cash.SERVICE_ID+"尚未生成，初始化金额CASH数值为空！");
			}
			if(!Real_Pay_Cash_str.equals("") || Real_Pay_Cash_str.trim().length()>0){
				factory.interruptTransaction();
				throw new Exception("账务管理销账，该账单"+cash.SERVICE_ID+"不存在，该账单可能已经被销账！");
			}
			
			int real_pay_cash_int = 0;
			int cash_int = Integer.parseInt(Cash);
			if(Free_Cash_str.equals("0") || Free_Cash_str.equals("") || Free_Cash_str.trim().length()==0){
				real_pay_cash_int = cash_int;
			}else{
				int free_cash_int = Integer.parseInt(Free_Cash_str);
				real_pay_cash_int = cash_int - free_cash_int;
				if(real_pay_cash_int <= 0){
					factory.interruptTransaction();
					throw new Exception("账务管理销账，账单"+cash.SERVICE_ID+"，预期交款总额："+cash_int+"，实际减免金额："+free_cash_int+"非法，预期赋予“实际交付钱数”不大于0，不合账务逻辑！");
				}
			}

			DateUtil du = new DateUtil();
			String serviceTableName = "T_Service_Cash_" + du.getStringTime("yyyyMM");

			String sql_cancle = "update "+serviceTableName+" set ";
			sql_cancle += " Real_Pay_Cash="+real_pay_cash_int+", Status = '1',Pay_Cash_SEQ="+pay_seq+",Oper_User='"+request.user_id+"',Oper_Time=sysdate";
			sql_cancle += " where Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Status=0";
			int update_count = factory.excute(sql_cancle);
			if(factory.SQL_Error_Code != 0){		
				throw new Exception("账务管理销账，改变终端（"+Imei_Number+","+Uim_Number+"）销账流水出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			if(update_count > 1){
				factory.interruptTransaction();
				throw new Exception("账务管理销账，改变终端（"+Imei_Number+","+Uim_Number+"）销账流水出错，改变个数（"+update_count+"）超出限制！");			
			}else if(update_count == 0){
				factory.interruptTransaction();
				throw new Exception("账务管理销账，没有查询到用于终端（"+Imei_Number+","+Uim_Number+"）销账账单，该终端可能已经被销账！");			
			}
		}
		
		
		String sql_query_cash_list = "select Service_ID,Msisdn,Cash,Real_Pay_Cash,Start_Time,End_Time,May_Free_Cash,Free_Cash,Status ";
		sql_query_cash_list += " from V_Service_Cash ";
		sql_query_cash_list += " where Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Msisdn='"+cash.MSISDN+"' and Status in ('0')";
		List<Map<String,Object>> list_query_cash = factory.queryList(sql_query_cash_list);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，查询用户更新前台销账列表数据（终端"+Imei_Number+","+Uim_Number+","+cash.MSISDN+"）出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，提交销账"+cash.SERVICE_ID+"事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"销账成功，销账流水号是（"+pay_seq+"）！");
		reponse.set("list", list_query_cash);
		return reponse;
	}
	
	private FlexReponse queryBillInfo(FlexRequest request) throws Exception{
		T_BILL_DETAIL bill = (T_BILL_DETAIL) request.getBean("bill");
//		String notbefore =  (String) request.getBean("notbefore");
//		String notafter =  (String) request.getBean("notafter");
		
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		String sql_map = "select Imei_Number,Uim_Number,Current_Hire_Time,Status from T_Terminal_Info where Msisdn='"+bill.MSISDN+"'";
		Map<String,Object> map_last_hire_map = factory.queryMap(sql_map);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，查询终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(map_last_hire_map == null){
			factory.interruptTransaction();
			throw new Exception("终端"+bill.MSISDN+"已经不存在！");			
		}
		String Imei_Number = (String) map_last_hire_map.get("IMEI_NUMBER");
		String Uim_Number = (String) map_last_hire_map.get("UIM_NUMBER");
		
		String sql = "select BILL_ID,Imei_Number,Uim_Number,Msisdn,Room_No,Hotel_ID,Broker_ID,Org_Rec_ID,Service_ID,Cash,Start_Time,End_Time,Create_Time,Free_Flag,Cash_Type,Status ";
		sql += " from V_Bill_Detail";
//		sql += " where Service_ID in (select distinct Service_ID from V_Service_Cash where Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Status=1 order by Service_ID) ";
//		sql += " where Create_Time>=to_date('"+notbefore+"','yyyy-mm-dd hh24:mi:ss') and Create_Time<=to_date('"+notafter+"','yyyy-mm-dd hh24:mi:ss') and Service_ID in (select distinct Service_ID from V_Service_Cash where Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"') ";
		sql += " where Service_ID in (select distinct Service_ID from V_Service_Cash where Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"') and Service_ID="+bill.SERVICE_ID+" ";
		sql += " order by Create_Time ";
		
		List<Map<String,Object>> list = factory.queryList(sql);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理，查看终端（"+Imei_Number+"，"+Uim_Number+"）详单数据出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，提交查看终端（"+Imei_Number+"，"+Uim_Number+"）详单数据出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(list.size()<1){
			throw new Exception("详单数据不存在！");
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		reponse.set("Msisdn", bill.MSISDN);
		return reponse;
	}
	
	private FlexReponse leadData(FlexRequest request) throws Exception{
		String xml = (String) request.getBean("xml");
		DBBuilder builder = new DBBuilder();
		String url = builder.leadDataToFile(xml, "BillData");
		if(builder.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，导出详单数据出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("url", url);
		return reponse;
	}
	
	private FlexReponse doFreeCashSettlementForHotel(FlexRequest request) throws Exception{
		T_SERVICE_CASH cash = (T_SERVICE_CASH) request.getBean("cash");
		String free_user_id = (String) request.getBean("free_user_id");
		String free_user_password = (String) request.getBean("free_user_password");
		String free_user_menu_id = (String) request.getBean("free_user_menu_id");
		
		DBBuilder builder = new DBBuilder();
		int count_free_user = builder.getCount("T_User_Info", "User_ID='"+free_user_id+"' and Password='"+free_user_password+"'");
		if(builder.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，查找将要减免费用的账单"+cash.SERVICE_ID+"出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		if(count_free_user == 0){
			throw new Exception("账务管理销账，费用减免授权账号"+free_user_id+"或密码错误！");
		}
		
		FlexRequest free_user_request = new FlexRequest();
		free_user_request.user_id = free_user_id;
		free_user_request.org_id = request.org_id;
		free_user_request.menu_id = free_user_menu_id;
		super.checkRole(free_user_request);
		
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		
		String sql_map = "select Imei_Number,Uim_Number,Current_Hire_Time,Status from T_Terminal_Info where Msisdn='"+cash.MSISDN+"'";
		Map<String,Object> map_last_hire_map = factory.queryMap(sql_map);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，查询终端信息出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(map_last_hire_map == null){
			factory.interruptTransaction();
			throw new Exception("账务管理销账，您要查询的终端"+cash.MSISDN+"已经不存在！");			
		}
		String Imei_Number = (String) map_last_hire_map.get("IMEI_NUMBER");
		String Uim_Number = (String) map_last_hire_map.get("UIM_NUMBER");
		
		DateUtil du = new DateUtil();
		String serviceTableName = "T_Service_Cash_" + du.getStringTime("yyyyMM");
		
		Map<String,Object> map_cash = factory.queryMap("select Cash,Real_Pay_Cash,May_Free_Cash,Free_Cash from V_Service_Cash where Service_ID="+cash.SERVICE_ID+" and Status='0'");
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，查找将要减免费用的账单"+cash.SERVICE_ID+"出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(map_cash == null){
			factory.interruptTransaction();
			throw new Exception("账务管理销账，将要减免费用的账单"+cash.SERVICE_ID+"已经不存在！");
		}
		
		int cash_int = Integer.parseInt((String) map_cash.get("CASH"));
		int may_cash_int = Integer.parseInt((String) map_cash.get("MAY_FREE_CASH"));
		int temp_free_cash_int = Integer.parseInt(cash.FREE_CASH);
		int real_pay_cash = cash_int - temp_free_cash_int;
		if(temp_free_cash_int > may_cash_int){
			factory.interruptTransaction();
			throw new Exception("账务管理销账，减免金额不能大于系统提供的可减免金额！");
		}
		if(real_pay_cash < 0){
			factory.interruptTransaction();
			throw new Exception("账务管理销账，减免金额不能大于预算总额！");
		}
		
//		String update_sql = "update "+serviceTableName+" set Real_Pay_Cash = "+real_pay_cash+" ,Free_Cash="+temp_free_cash_int+" ,Advance_User='"+request.user_id+"'";
		String update_sql = "update "+serviceTableName+" set Free_Cash="+temp_free_cash_int+" ,Advance_User='"+request.user_id+"'";
		update_sql += " where Service_ID="+cash.SERVICE_ID+" and Status=0 ";
		int update_count = factory.excute(update_sql);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，减免账单"+cash.SERVICE_ID+"费用出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		if(update_count != 1){
			factory.interruptTransaction();
			throw new Exception("账务管理销账，即将减免费用的账单"+cash.SERVICE_ID+"已经不存在，或已经被修改，请重新查询！（原始改变记录数："+update_count+"）");
		}
		
		String sql_query_cash_list = "select Service_ID,Msisdn,Cash,Real_Pay_Cash,Start_Time,End_Time,May_Free_Cash,Free_Cash,Status ";
		sql_query_cash_list += " from V_Service_Cash ";
		sql_query_cash_list += " where Imei_Number='"+Imei_Number+"' and Uim_Number='"+Uim_Number+"' and Msisdn='"+cash.MSISDN+"' and Status in ('0')";
		List<Map<String,Object>> list_query_cash = factory.queryList(sql_query_cash_list);
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，查询用户更新前台销账列表数据（终端"+Imei_Number+","+Uim_Number+","+cash.MSISDN+"）出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){		
			throw new Exception("账务管理销账，提交销账"+cash.SERVICE_ID+"事务出错，原因："+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"账单减免费用成功，可以继续结账操作！");
		reponse.set("list", list_query_cash);
		return reponse;
	}
	
	private FlexReponse statisticsData(FlexRequest request) throws Exception{
//		String xml = (String) request.getBean("xml");
//		DBBuilder builder = new DBBuilder();
//		String url = builder.leadDataToFile(xml, "BillData");
//		if(builder.SQL_Error_Code != 0){		
//			throw new Exception("账务管理销账，导出详单数据出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
//		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		return reponse;
	}
}
