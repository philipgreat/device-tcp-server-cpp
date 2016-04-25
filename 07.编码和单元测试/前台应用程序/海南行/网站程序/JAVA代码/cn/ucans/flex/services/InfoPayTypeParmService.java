package cn.ucans.flex.services;

import java.util.List;
import java.util.Map;
import chobits.db.DBBuilder;
import chobits.db.DBFactory;
import chobits.log.Logger;
import cn.ucans.bean.T_INFO_PAY_TYPE;
import cn.ucans.bean.T_INFO_PAY_TYPE_PARM;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;

public class InfoPayTypeParmService extends FlexService{
	
	private Logger logger = new Logger(TerminalTypeService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------套餐费率设置管理任务 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 9040:
					reponse = this.initInfoPayTypeParmService(request);
					break;
				case 9041:
					reponse = this.addInfoPayTypeParmService(request);
					break;
				case 9042:
					reponse = this.queryInfoPayTypeParmService(request);
					break;
				case 9043:
					reponse = this.updateInfoPayTypeParmService(request);
					break;
				case 9044:
					reponse = this.deleteInfoPayTypeParmService(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------套餐费率设置管理任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------套餐费率设置管理任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initInfoPayTypeParmService(FlexRequest request) throws Exception{
		
		String select_InfoPayTypeParm_Info="Select a.Pay_Type,b.Pay_Name,a.Start_Number,a.End_Number,a.Charge_Type,a.Pay_Rate,a.Notes,b.Create_Time From T_Info_Pay_Type_Parm a,T_Info_Pay_Type b where a.Pay_Type=b.Pay_Type";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_InfoPayTypeParm_Info);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("查询套餐配置信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse addInfoPayTypeParmService(FlexRequest request) throws Exception{
		DBFactory dbFactory = new DBFactory();
		
		T_INFO_PAY_TYPE_PARM payTypeParmForAdd = (T_INFO_PAY_TYPE_PARM) request.getBean("payTypeParmForAdd");
		T_INFO_PAY_TYPE payTypeForAdd = (T_INFO_PAY_TYPE) request.getBean("payTypeForAdd");
		
		dbFactory.openTransaction();
		
		String sql="";
		
		int count=dbFactory.getCount("T_Info_Pay_Type", " Pay_Name='"+payTypeForAdd.PAY_NAME+"'");
		int payType=0;
		
		if(count > 0){
			
			payType= dbFactory.queryIntClumn("select Pay_Type from T_Info_Pay_Type where Pay_Name='"+ payTypeForAdd.PAY_NAME+"'");
			if(dbFactory.SQL_Error_Code != 0){	
				throw new Exception("套餐配置，根据套餐名称得到套餐类型出错，原因："+dbFactory.SQL_Error_Code+" "+dbFactory.SQL_Error_Message);
			}
			int countForStartEndNumber=dbFactory.getCount("T_Info_Pay_Type_Parm", " Pay_Type="+ payType + " and ((Start_Number <= "+payTypeParmForAdd.START_NUMBER+ " and End_Number>="+payTypeParmForAdd.START_NUMBER +") " +
					"or (Start_Number <= "+payTypeParmForAdd.END_NUMBER+ " and End_Number>="+payTypeParmForAdd.END_NUMBER +") or ( Start_Number > "+payTypeParmForAdd.START_NUMBER+ " and End_Number<"+payTypeParmForAdd.END_NUMBER +" )) " );
			
			if(dbFactory.SQL_Error_Code != 0){	
				throw new Exception("套餐配置，判断套餐设定区间出错，原因："+dbFactory.SQL_Error_Code+" "+dbFactory.SQL_Error_Message);
			}
			
			if(countForStartEndNumber == 0){
				
				if("-1".equals(payTypeParmForAdd.END_NUMBER)){
					payTypeParmForAdd.END_NUMBER="999999999";
				}
				
				sql = "Insert into T_Info_Pay_Type_Parm(Pay_Type,Start_Number,End_Number,Charge_Type,Pay_Rate,Notes,Create_Time) Values("+payType+","
					+payTypeParmForAdd.START_NUMBER+ ","+payTypeParmForAdd.END_NUMBER+",'" + payTypeParmForAdd.CHARGE_TYPE +"'," + payTypeParmForAdd.PAY_RATE+",'" + payTypeParmForAdd.NOTES+ "',sysdate)";
				dbFactory.excute(sql);
				if(dbFactory.SQL_Error_Code != 0){	
					throw new Exception("添加套餐配置信息出错，原因："+dbFactory.SQL_Error_Code+" "+dbFactory.SQL_Error_Message);
				}
			}else{
				dbFactory.interruptTransaction();
				throw new Exception("由于在套餐设定区间重复，添加套餐配置信息出错!!请重新输入！！");
			}
			
		}else{
			
			if("-1".equals(payTypeParmForAdd.END_NUMBER)){
				payTypeParmForAdd.END_NUMBER="999999999";
			}
			
			int ID= dbFactory.queryIntClumn("select max(Pay_Type)+1 from T_Info_Pay_Type");
			sql = "Insert Into T_Info_Pay_Type(Pay_Type,Pay_Name,User_ID,Create_Time) Values("+ID+",'"
				+payTypeForAdd.PAY_NAME+ "','"+request.user_id+"',sysdate)";
			if(dbFactory.SQL_Error_Code != 0){	
				throw new Exception("套餐配置，取得套餐类型ID，原因："+dbFactory.SQL_Error_Code+" "+dbFactory.SQL_Error_Message);
			}
			
			String sql2 = "Insert into T_Info_Pay_Type_Parm(Pay_Type,Start_Number,End_Number,Charge_Type,Pay_Rate,Notes,Create_Time) Values("+ ID +","
			+payTypeParmForAdd.START_NUMBER+ ","+payTypeParmForAdd.END_NUMBER+",'" + payTypeParmForAdd.CHARGE_TYPE +"'," + payTypeParmForAdd.PAY_RATE+",'" + payTypeParmForAdd.NOTES+ "',sysdate)";
			
			dbFactory.excute(sql);
			
			if(dbFactory.SQL_Error_Code != 0){	
				throw new Exception("添加套餐配置信息出错，原因："+dbFactory.SQL_Error_Code+" "+dbFactory.SQL_Error_Message);
			}
			
			dbFactory.excute(sql2);
			
			if(dbFactory.SQL_Error_Code != 0){	
				throw new Exception("添加套餐配置信息出错，原因："+dbFactory.SQL_Error_Code+" "+dbFactory.SQL_Error_Message);
			}
			
		}	
		dbFactory.closeTransaction();
		if(dbFactory.SQL_Error_Code != 0){	
			throw new Exception("套餐配置，提交添加套餐配置事务出错，原因："+dbFactory.SQL_Error_Code+" "+dbFactory.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"添加套餐配置信息成功！");
		return reponse;
	}
	
	
	private FlexReponse queryInfoPayTypeParmService(FlexRequest request) throws Exception{
		String pay_Name = (String) request.getBean("Pay_Name");
//		String startTime = (String) request.getBean("startTime");
//		String endTime = (String) request.getBean("endTime");
		
		String select_PayTypeParm="Select a.Pay_Type,b.Pay_Name,a.Start_Number,a.End_Number,a.Charge_Type,a.Pay_Rate,a.Notes,b.Create_Time From T_Info_Pay_Type_Parm a,T_Info_Pay_Type b where a.Pay_Type=b.Pay_Type";
		
		if(pay_Name != null && !("").equals(pay_Name)){
			select_PayTypeParm+=" and b.Pay_Name like '%"+pay_Name+"%'";
		}
//		if(startTime !=null && !("").equals(startTime)){	
//			select_PayTypeParm+= " and to_char(b.Create_Time,'yyyy-mm-dd') >='"+ startTime+"'";
//		}
//		if(endTime !=null && !("").equals(endTime)){NI
//			select_PayTypeParm+=" and to_char(b.Create_Time,'yyyy-mm-dd') <='"+endTime+"'";
//		}
		
		select_PayTypeParm+=" Order by b.Pay_Name,a.Start_Number,a.End_Number";
		
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_PayTypeParm);;
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("查询套餐配置信息，获取套餐配置信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	
	private FlexReponse updateInfoPayTypeParmService(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();

		T_INFO_PAY_TYPE_PARM payTypeParmForUpdate = (T_INFO_PAY_TYPE_PARM) request.getBean("payTypeParmForUpdate");
		T_INFO_PAY_TYPE_PARM oldPay_Type_Parm_Util = (T_INFO_PAY_TYPE_PARM) request.getBean("oldPay_Type_Parm_Util");
		
		if("-1".equals(payTypeParmForUpdate.END_NUMBER)){
			payTypeParmForUpdate.END_NUMBER="999999999";
		}
		
		int count = builder.getCount("T_Info_Pay_Type_Parm", "Pay_Type="+payTypeParmForUpdate.PAY_TYPE+" and Start_Number="+oldPay_Type_Parm_Util.START_NUMBER+" and End_Number="+oldPay_Type_Parm_Util.END_NUMBER +" and Charge_Type='"+oldPay_Type_Parm_Util.CHARGE_TYPE+"' and Pay_Rate='"+oldPay_Type_Parm_Util.PAY_RATE+"'");
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("修改套餐配置，获取套餐配置信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count<=0){
				throw new Exception("修改套餐配置，该套餐配置已经不存在或已经被修改，请确认后再修改！");
			}
		}
		
		String update_sql="";
		int countN = builder.getCount("T_Info_Pay_Type_Parm", "Pay_Type="+payTypeParmForUpdate.PAY_TYPE);
		if(1==countN){
			update_sql = "update T_Info_Pay_Type_Parm set ";
			update_sql += " Start_Number ="+payTypeParmForUpdate.START_NUMBER+",";
			update_sql += " End_Number ="+payTypeParmForUpdate.END_NUMBER+",";
			update_sql += " Charge_Type ='"+payTypeParmForUpdate.CHARGE_TYPE+"',";
			update_sql += " Pay_Rate ="+payTypeParmForUpdate.PAY_RATE+",";
			update_sql += " Notes ='"+payTypeParmForUpdate.NOTES+"',";
			update_sql += " Create_Time =sysdate";
			update_sql += " where Pay_Type="+payTypeParmForUpdate.PAY_TYPE+" and Start_Number="+oldPay_Type_Parm_Util.START_NUMBER+" and End_Number="+oldPay_Type_Parm_Util.END_NUMBER;
			builder.excute(update_sql);
			
			if(builder.SQL_Error_Code != 0){			
				throw new Exception("添加套餐配置信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}
		}else{
			
			
			int countForStartEndNumber=builder.getCount("T_Info_Pay_Type_Parm", " Pay_Type="+ payTypeParmForUpdate.PAY_TYPE + " and ((Start_Number <= "+payTypeParmForUpdate.START_NUMBER+ " and End_Number>="+payTypeParmForUpdate.START_NUMBER +") or (Start_Number <= "
					
					+payTypeParmForUpdate.END_NUMBER+ " and End_Number>="+payTypeParmForUpdate.END_NUMBER +") or ( Start_Number > "+payTypeParmForUpdate.START_NUMBER+ " and End_Number<"+payTypeParmForUpdate.END_NUMBER +" )) and ((Start_Number!="+ oldPay_Type_Parm_Util.START_NUMBER + ") and (End_Number!="+ oldPay_Type_Parm_Util.END_NUMBER +"))");

//			int countForStartEndNumber=builder.getCount("T_Info_Pay_Type_Parm", " Pay_Type="+ payTypeParmForUpdate.PAY_TYPE + " and (((Start_Number > "+payTypeParmForUpdate.END_NUMBER+ ") or( End_Number<"+payTypeParmForUpdate.START_NUMBER +")) and ((Start_Number!="+ oldPay_Type_Parm_Util.START_NUMBER + ") and (End_Number!="+ oldPay_Type_Parm_Util.END_NUMBER +")))" );
			
			
			if(countForStartEndNumber==0){
				
				update_sql = "update T_Info_Pay_Type_Parm set ";
				update_sql += " Start_Number ="+payTypeParmForUpdate.START_NUMBER+",";
				update_sql += " End_Number ="+payTypeParmForUpdate.END_NUMBER+",";
				update_sql += " Charge_Type ='"+payTypeParmForUpdate.CHARGE_TYPE+"',";
				update_sql += " Pay_Rate ="+payTypeParmForUpdate.PAY_RATE+",";
				update_sql += " Notes ='"+payTypeParmForUpdate.NOTES+"',";
				update_sql += " Create_Time =sysdate";
				update_sql += " where Pay_Type="+payTypeParmForUpdate.PAY_TYPE+" and Start_Number="+oldPay_Type_Parm_Util.START_NUMBER+" and End_Number="+oldPay_Type_Parm_Util.END_NUMBER;
				builder.excute(update_sql);
				
				if(builder.SQL_Error_Code != 0){			
					throw new Exception("添加套餐配置信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
				}
			}else{
					throw new Exception("由于在套餐设定区间重复，修改套餐配置信息出错!!请重新输入！！");
			}
			
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"修改套餐配置信息成功！");
		
		return reponse;
	}
	
	private FlexReponse deleteInfoPayTypeParmService(FlexRequest request) throws Exception{
		DBFactory dbFactory = new DBFactory();
		dbFactory.openTransaction();
		T_INFO_PAY_TYPE_PARM infoPayTypeParmForDel = (T_INFO_PAY_TYPE_PARM) request.getBean("infoPayTypeParmForDel");
		
		
		int countN = dbFactory.getCount("T_Info_Pay_Type_Parm", "Pay_Type="+infoPayTypeParmForDel.PAY_TYPE+" and Start_Number="+infoPayTypeParmForDel.START_NUMBER+" and End_Number="+infoPayTypeParmForDel.END_NUMBER +" and Charge_Type='"+infoPayTypeParmForDel.CHARGE_TYPE+"' and Pay_Rate='"+infoPayTypeParmForDel.PAY_RATE+"'");
		
		if(dbFactory.SQL_Error_Code != 0){			
			throw new Exception("删除套餐配置，获取套餐配置信息出错，原因："+dbFactory.SQL_Error_Code+" "+dbFactory.SQL_Error_Message);
		}else{
			if(countN<=0){
				dbFactory.interruptTransaction();
				throw new Exception("删除套餐配置，该套餐配置已经不存在或已经被修改，请确认后再删除！");
			}
		}
		
		
		int count = dbFactory.getCount("T_Info_Pay_Type_Parm", "Pay_Type="+infoPayTypeParmForDel.PAY_TYPE);
		if(count>0){
			if(count>1){
				String sql = "delete from T_Info_Pay_Type_Parm where Pay_Type ="+infoPayTypeParmForDel.PAY_TYPE +" and Start_Number="+infoPayTypeParmForDel.START_NUMBER+" and End_Number="+infoPayTypeParmForDel.END_NUMBER;
				dbFactory.excute(sql);
			}else if(count==1){
				String sql2 = "delete from T_Info_Pay_Type_Parm where Pay_Type ="+infoPayTypeParmForDel.PAY_TYPE +" and Start_Number="+infoPayTypeParmForDel.START_NUMBER+" and End_Number="+infoPayTypeParmForDel.END_NUMBER;
				String sql3 = "delete from T_Info_Pay_Type where Pay_Type =" + infoPayTypeParmForDel.PAY_TYPE;
				dbFactory.excute(sql2);
				if(dbFactory.SQL_Error_Code != 0){	
					throw new Exception("删除套餐配置出错，原因："+dbFactory.SQL_Error_Code+" "+dbFactory.SQL_Error_Message);
				}
				dbFactory.excute(sql3);
				if(dbFactory.SQL_Error_Code != 0){	
					throw new Exception("删除套餐配置出错，原因："+dbFactory.SQL_Error_Code+" "+dbFactory.SQL_Error_Message);
				}
			}
		}else{
			if(dbFactory.SQL_Error_Code != 0){	
				throw new Exception("要删除套餐配置已经不存在或已经被修改，请确认后再删除！！！");
			}
		}
		dbFactory.closeTransaction();
		if(dbFactory.SQL_Error_Code != 0){	
			throw new Exception("提交删除套餐配置事务出错，原因："+dbFactory.SQL_Error_Code+" "+dbFactory.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"删除套餐配置信息成功！");
		return reponse;
	}
}
