package cn.ucans.flex.services;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import jxl.Workbook;
import chobits.db.DBBuilder;
import chobits.db.DBFactory;
import chobits.init.ChobitsConfig;
import chobits.log.Logger;
import cn.ucans.bean.T_INFO_NUMBER_OBJECT;
import cn.ucans.bean.T_INFO_SEND_TASK;
import cn.ucans.bean.T_INFO_SEND_TASK_HIS;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class TaskInfoSendService extends FlexService{
	
	private Logger logger = new Logger(TaskInfoSendService.class.getName());
	String strFileData = "";
	String strStyle = "";
	String strFileName = "";
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------信息发布任务 启动-------------------------------");
		FlexReponse reponse = null;
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 2013:
					reponse = this.queryHotelNameNoAll(request);
					break;
				case 2014:
					reponse = this.queryT_Info_Language_List(request);
					break;
				case 2015:
					reponse = this.queryT_Org_InfoWhere(request);
					break;
				case 2016:
					reponse = this.takeT_Info_Number_Object(request);
					break;
				case 2017:
					reponse = this.takeT_Info_Number_ObjectCount(request);
					break;
				case 2018:
					reponse = this.queryT_Info_Send_Task(request);
					break;
				case 2019:
//					System.out.println("queryT_Info_Number_ObjectqueryT_Info_Number_ObjectqueryT_Info_Number_Object22222222222222222222222");
					reponse = this.queryT_Info_Number_Object(request);
					break;
//				case 2020:
//					System.out.println("T_Info_TypeT_Info_TypeT_Info_TypeT_Info_Type111111111111111111111111111111111111");
//					reponse = this.queryT_Info_Type(request);
//					break;
				case 2021:
					reponse = this.queryT_Info_Number_ObjectSulstion(request);
					break;
				case 2022:
					reponse = this.insertT_Info_Send_Task(request);
					break;
				case 2023:
					reponse = this.updateT_Info_Send_Task(request);
					break;
				case 2024:
					reponse = this.delT_Info_Send_Task(request);
					break;
				case 2025:
					reponse = this.submitT_Info_Send_Task(request);
					break;
				case 2026:
					reponse = this.suspensionT_Info_Send_Task(request);
					break;
				case 2027:
					reponse = this.CancelsSuspensionT_Info_Send_Task(request);
					break;
				case 2028:
					reponse = this.updateT_Info_Send_TaskAll(request);
					break;
				case 2029:
					reponse = this.stopSuspensionT_Info_Send_Task(request);
					break;
				case 2030:
					reponse = this.continueSuspensionT_Info_Send_Task(request);
					break;
				case 2031:
					reponse = this.queryT_Info_Send_Task_HIS(request);
					break;
				case 2032:
					reponse = this.queryTInfoType_ORGNAME(request);
					break;
				case 2033:
					reponse = this.takeHotelTiminerPhoneNumber(request);
					break;
				case 2034:
					reponse = this.CreateExcelFile(strFileData,strStyle,request);
					break;
				case 2035:
					reponse = this.DeleteExcelFile(strFileName, request);
					break;
				case 2036:
					reponse = this.verificationT_Info_Send_Task(request);
					break;
				case 2037:
					reponse = this.verificationPassesT_Info_Send_Task(request);
					break;
				case 2038:
					reponse = this.verificationBackT_Info_Send_Task(request);
					break;
				case 2039:
					reponse = this.verificationCancelT_Info_Send_Task(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------信息发布任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------信息发布任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	public FlexReponse CreateExcelFile(String strFileData,String strStyle,FlexRequest request) throws Exception 
	{
		
		File	oFile = null;
		String	strLine;
		String	strCol;
		String	strData = strFileData;
		int		nLenPlace = 0;
		int		nColPlace;

		try 
		{
			File	oDir = new File(ChobitsConfig.TempFileForderPath);
//			File	oDir = new File("/Tomcat/webapps/CCSS/XLSTmepFile");
//			File	oDir = new File("F:/MyEclipseWorkSpace/Souce/Happy_Space/Happy/WebRoot/TempFile");
			oFile = File.createTempFile("Excel",".xls",oDir);
			jxl.write.WritableWorkbook wwb = Workbook.createWorkbook(oFile);
			jxl.write.WritableSheet ws = wwb.createSheet("终端手机号码", 0);
			for(int nLine = 0;nLenPlace != -1;nLine ++)
			{
				nLenPlace  = strData.indexOf("&&");
				if(nLenPlace == -1)
				{
					strLine = strData;
				}
				else
				{
					strLine = strData.substring(0,nLenPlace);
					strData = strData.substring(nLenPlace + 2);
				}
				nColPlace = 0;
				for(int nCol = 0;nColPlace != -1;nCol ++)
				{
					nColPlace = strLine.indexOf("||");
					if(nColPlace == -1)
					{
						strCol = strLine;
					}
					else
					{
						strCol = strLine.substring(0,nColPlace);
						strLine = strLine.substring(nColPlace + 2);
					}
					jxl.write.Label labelC = new jxl.write.Label(nCol,nLine, strCol);
					ws.addCell(labelC);	
				}
			}
			nLenPlace = 0;
			for(;nLenPlace != -1;)
			{
				nLenPlace  = strStyle.indexOf("&");
				if(nLenPlace == -1)
				{
					strLine = strData;
				}
				else
				{
					strLine = strData.substring(0,nLenPlace);
					strData = strData.substring(nLenPlace + 2);
				}
			}
			wwb.write();
			wwb.close();
			String fileName = oFile.getName();
			FlexReponse reponse = new FlexReponse(request,0,"");
			reponse.org_id = fileName;
			return reponse;
		}
		catch (Exception e)
		{
			String error = "创建xls文件发生错误 -- " + e.toString();
			 throw new Exception(error);
		}
	}
	
	public FlexReponse DeleteExcelFile(String strFileName,FlexRequest request) throws Exception 
	{
		String	strLine;
		try 
		{		
			java.io.File DelFile=new java.io.File(ChobitsConfig.TempFileForderPath+"/" + strFileName);
//			java.io.File DelFile=new java.io.File("F:/MyEclipseWorkSpace/Souce/Happy_Space/Happy/WebRoot/TempFile/" + strFileName);
			DelFile.delete();
			FlexReponse reponse = new FlexReponse(request,0,"删除xls成功！");
			return reponse;
		}
		catch (Exception e)
		{
			String error = "删除xls文件发生错误 -- "+ e.toString();
			throw new Exception(error);
		}
	}
	private FlexReponse queryHotelNameNoAll(FlexRequest request) throws Exception{
		ParamHandler builder = new ParamHandler();
		List<Map<String,Object>> list  = builder.get_T_Org_InfoNoAll("");
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryT_Info_Language_List(FlexRequest request) throws Exception{
		ParamHandler builder = new ParamHandler();
		List<Map<String,Object>> list  = builder.getLanguageName("");
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse queryT_Org_InfoWhere(FlexRequest request) throws Exception{
		ArrayList list1 = (ArrayList)request.getBean("arrHotelLevel");
		ParamHandler builder = new ParamHandler();
		List<Map<String,Object>> list  = builder.getT_Org_InfoHoteName(list1);
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse takeT_Info_Number_Object(FlexRequest request) throws Exception{
		T_INFO_NUMBER_OBJECT sendObject = (T_INFO_NUMBER_OBJECT)request.getBean("sendObject");
		ArrayList hotelLevelList = (ArrayList)request.getBean("arrHotelLevel");
		ArrayList hotelNameList = (ArrayList)request.getBean("arrHotelName");
		String Hotel_Grade_data = "";
		int count = 0;
		int countAll = 0;
		FlexReponse reponse = null;
		String sql = "";
		List<Map<String,Object>> listNewLevel = new  ArrayList<Map<String,Object>>();
		if(hotelLevelList.size()>0){
			ParamHandler builder = new ParamHandler();
			listNewLevel  = builder.getT_Org_InfoHoteName(hotelLevelList);
		}
		if(listNewLevel.size()>0){
			for(int i = 0;i<listNewLevel.size();i++){
				Map map = (Map)listNewLevel.get(i);	
				Hotel_Grade_data +="'"+map.get("data")+"',";
			}
		}
		if(hotelNameList.size()>0){
			for(int i = 0;i<hotelNameList.size();i++){
				Map map = (Map)hotelNameList.get(i);	
				Hotel_Grade_data +="'"+map.get("data")+"',";
			}
		}
		if(Hotel_Grade_data.length() > 0){
			Hotel_Grade_data =  Hotel_Grade_data.substring(0, Hotel_Grade_data.length()-1);
		}else{
			Hotel_Grade_data = "''";
		}
		DBBuilder builder = new DBBuilder();
		long sendGroupID = builder.getOracleSequreNextValue("S_Info");
		DBFactory factory = new DBFactory();
		factory.openTransaction();
//		提取号码并创建发送对象_"+sendObject.INSERT_TIME+"
		sql = "insert into T_Info_Number_List_"+sendObject.INSERT_TIME+"(Telephone_Number,Send_Group_ID) select Msisdn,"+sendGroupID+" from T_Terminal_Info "+"" +
				" where Status='12' and Language='"+sendObject.LANGUAGE+"' and Hotel_ID in("+Hotel_Grade_data+")";
		count = factory.excute(sql);
		if(count<1){
			factory.interruptTransaction();
			reponse = new FlexReponse(request,0,"根据您的提取条件没有找到相应的终端手机号码！");
			return reponse;
		}
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("创建发送对象时出错，错误代码："+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
		}
//		 获取创建发送对象的号码个数
		countAll = factory.getCount("T_Info_Number_List_"+sendObject.INSERT_TIME+"", "Send_Group_ID = "+sendGroupID+"");
//		发送对象信息
		sql = "insert into T_Info_Number_Object(Send_Group_ID,Send_Group_Name,Amount,Language,Mode_Type,User_ID,Org_ID,Insert_Time)"+"" +
				"values("+sendGroupID+",'"+sendObject.SEND_GROUP_NAME+"',"+countAll+",'"+sendObject.LANGUAGE+"','0','"+sendObject.USER_ID+"','"+sendObject.ORG_ID+"',sysdate)";
		factory.excute(sql);
		if(factory.SQL_Error_Code == 1){	
			factory.interruptTransaction();
			reponse = new FlexReponse(request,0,"发送清单名称已存在,请确认重新录入！");
			return reponse;
		}
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("增加发送对象信息时－－错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
		}
//		查询登录者的组织创建的发送对象的信息及个数
		List<Map<String,Object>> list = factory.queryList("select Send_Group_Name Send_Group_Name,to_char(Amount) Amount from T_Info_Number_Object where Org_ID='"+sendObject.ORG_ID+"' order by Insert_Time desc");
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
		}
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
		}
		reponse = new FlexReponse(request,0,"提取成功！");
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse takeT_Info_Number_ObjectCount(FlexRequest request) throws Exception{
		T_INFO_NUMBER_OBJECT sendObject = (T_INFO_NUMBER_OBJECT)request.getBean("sendObject");
		ArrayList hotelLevelList = (ArrayList)request.getBean("arrHotelLevel");
		ArrayList hotelNameList = (ArrayList)request.getBean("arrHotelName");
		String Hotel_Grade_data = "";
		int count = 0;
		FlexReponse reponse = null;
		List<Map<String,Object>> listNewLevel = new  ArrayList<Map<String,Object>>();
		if(hotelLevelList.size()>0){
			ParamHandler builder = new ParamHandler();
			listNewLevel  = builder.getT_Org_InfoHoteName(hotelLevelList);
		}
		if(listNewLevel.size()>0){
			for(int i = 0;i<listNewLevel.size();i++){
				Map map = (Map)listNewLevel.get(i);	
				Hotel_Grade_data +="'"+map.get("data")+"',";
			}
		}
		if(hotelNameList.size()>0){
			for(int i = 0;i<hotelNameList.size();i++){
				Map map = (Map)hotelNameList.get(i);	
				Hotel_Grade_data +="'"+map.get("data")+"',";
			}
		}
		if(Hotel_Grade_data.length() > 0){
			Hotel_Grade_data =  Hotel_Grade_data.substring(0, Hotel_Grade_data.length()-1);
		}else{
			Hotel_Grade_data = "''";
		}
		DBBuilder builder = new DBBuilder();
		count = builder.getCount("T_Terminal_Info", "Status='12' and Language='"+sendObject.LANGUAGE+"' and Hotel_ID in("+Hotel_Grade_data+")");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		reponse = new FlexReponse(request,0,"");
		reponse.set("count", count);
		return reponse;
	}
	private FlexReponse queryT_Info_Send_Task(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("sendTask");
		String sql = "";
		DBBuilder builder = new DBBuilder();
		sql = "select Task_ID,Task_Name,Content,Send_Group_ID,Language,Info_Type,Start_Time,End_Time,Pay_Type,Status,Send_Plan,Send_Already,Priority,Return_Report,Flow_Rate,User_ID,Org_ID,Create_Time,Auditor_ID,Audit_Time from T_Info_Send_Task"+
				" where to_char(Create_Time,'yyyy-mm-dd hh24:mi:ss')>='"+sendTask.START_TIME+"' and to_char(Create_Time,'yyyy-mm-dd hh24:mi:ss')<='"+sendTask.END_TIME+"'";
//		and Org_ID='"+sendTask.ORG_ID+"' 
		if(sendTask.TASK_NAME != "" && sendTask.TASK_NAME.length()>0){
			sql+=" and Task_Name like '%"+sendTask.TASK_NAME+"%'";
		}
		if(sendTask.STATUS != "" && sendTask.STATUS.length()>0){
			sql+=" and STATUS = '"+sendTask.STATUS+"'";
		}
		if(sendTask.ORG_ID != "" && sendTask.ORG_ID.length()>0){
			sql+= " and Org_ID='"+sendTask.ORG_ID+"'" ;
		}
		if(sendTask.USER_ID != "" && sendTask.USER_ID.length()>0){
			sql+= " and User_ID='"+sendTask.USER_ID+"'" ;
		}
		sql+= "order by Create_Time desc";
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"");
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryT_Info_Number_Object(FlexRequest request) throws Exception{
		ParamHandler builder = new ParamHandler();
		List<Map<String,Object>> list  = builder.getT_Info_Number_Object("");
		List<Map<String,Object>> list1  = builder.getT_Info_Type("");
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		reponse.set("list1", list1);
		return reponse;
	}
	private FlexReponse queryT_Info_Type(FlexRequest request) throws Exception{
		ParamHandler builder = new ParamHandler();
		List<Map<String,Object>> list  = builder.getT_Info_Type("");
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryT_Info_Number_ObjectSulstion(FlexRequest request) throws Exception{
		T_INFO_NUMBER_OBJECT sendObject = (T_INFO_NUMBER_OBJECT)request.getBean("sendObject");
		ParamHandler builder = new ParamHandler();
		List<Map<String,Object>> list  = builder.getT_Info_Number_ObjectInfo(sendObject);
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse insertT_Info_Send_Task(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("sendTask");
		String sql = "";
		DBBuilder builder = new DBBuilder();
		long sendGroupID = builder.getOracleSequreNextValue("S_Info");
		sql = "insert into T_Info_Send_Task(Task_ID,Task_Name,Content,Send_Group_ID,Language,Info_Type,Start_Time,End_Time,Status,Send_Plan,Priority,Return_Report,User_ID,Org_ID,Create_Time)"+
				" select "+sendGroupID+",'"+sendTask.TASK_NAME+"','"+sendTask.CONTENT+"',Send_Group_ID,Language,"+sendTask.INFO_TYPE+",to_date('"+sendTask.START_TIME+"','yyyy-mm-dd hh24:mi:ss')," +
						"to_date('"+sendTask.END_TIME+"','yyyy-mm-dd hh24:mi:ss'),'0',Amount,9,1,'"+sendTask.USER_ID+"','"+sendTask.ORG_ID+"',sysdate from T_Info_Number_Object where Send_Group_ID="+sendTask.SEND_GROUP_ID+"";
		builder.excute(sql);
		if(builder.SQL_Error_Code == 1){
			throw new Exception("任务名称已存在，请重新录入！");
		}
		if(builder.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"任务制定成功!");
		return reponse;
	}
	private FlexReponse updateT_Info_Send_Task(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("sendTask");
		ParamHandler handler = new ParamHandler();
		T_INFO_NUMBER_OBJECT sendObject = new T_INFO_NUMBER_OBJECT();
		List listSendObject = handler.queryT_Info_Number_Object(sendTask.SEND_GROUP_ID);
		String sql = "";
		if(listSendObject.size() < 1){
			throw new Exception("查询发送对象失败，请重新修改！");
		}else{
			sendObject = (T_INFO_NUMBER_OBJECT)listSendObject.get(0);
		}
//		System.out.println(sendTask.STATUS+"-------------------------------------------------00000000000000000000000");
			 sql = "update T_Info_Send_Task set Task_Name='"+sendTask.TASK_NAME+"',Content='"+sendTask.CONTENT+"',Send_Group_ID="+sendTask.SEND_GROUP_ID+",Language='"+sendObject.LANGUAGE+"'" +
					",Info_Type='"+sendTask.INFO_TYPE+"',Start_Time=to_date('"+sendTask.START_TIME+"','yyyy-mm-dd hh24:mi:ss'),End_Time=to_date('"+sendTask.END_TIME+"','yyyy-mm-dd hh24:mi:ss'),Status='0'" +
					",Send_Plan="+sendObject.AMOUNT+" where Task_ID ='"+sendTask.TASK_ID+"'";
		System.out.println(sql);
		DBBuilder builder = new DBBuilder();
		
		builder.excute(sql);
		if(builder.SQL_Error_Code == 1){
			throw new Exception("任务名称已存在，请重新录入！");
		}
		if(builder.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"任务修改成功!");
		return reponse;
	}
	private FlexReponse delT_Info_Send_Task(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("delSendTask");
		String sql = "";
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		FlexReponse reponse = null;
//		--6.任务发送中超时;7.暂停后取消发布
//		--8.取消待发送任务;9.挂起后取消发布
		if(sendTask.STATUS.equals("0")){
			sql = "insert into T_Info_Send_Task_His(Task_ID,Task_Name,Content,Send_Group_ID,Language,Info_Type,Start_Time,End_Time,Pay_Type,Priority,Return_Report,Flow_Rate,User_ID,Create_Time,Auditor_ID,Audit_Time,Record_Cause,Statistics_Flag,Insert_Time,Send_Plan,Send_Already,Org_ID)" +
					"select Task_ID,Task_Name,Content,Send_Group_ID,Language,Info_Type,Start_Time,End_Time,Pay_Type,Priority,Return_Report,Flow_Rate,User_ID,Create_Time,Auditor_ID,Audit_Time,'"+"被删除"+"',"+0+",sysdate,Send_Plan,Send_Already,Org_ID from T_Info_Send_Task where Task_ID="+sendTask.TASK_ID+"";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
			}
		    sql = "delete T_Info_Send_Task where Task_ID = "+sendTask.TASK_ID+"";
		    factory.excute(sql);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
			}
			factory.closeTransaction();
			 reponse = new FlexReponse(request,0,"<"+sendTask.TASK_NAME+">任务已删除!");
		}
		if(sendTask.STATUS.equals("3")){
			sql = "insert into T_Info_Send_Task_His(Task_ID,Task_Name,Content,Send_Group_ID,Language,Info_Type,Start_Time,End_Time,Pay_Type,Priority,Return_Report,Flow_Rate,User_ID,Create_Time,Auditor_ID,Audit_Time,Record_Cause,Statistics_Flag,Insert_Time,Send_Plan,Send_Already,Org_ID)" +
					"select Task_ID,Task_Name,Content,Send_Group_ID,Language,Info_Type,Start_Time,End_Time,Pay_Type,Priority,Return_Report,Flow_Rate,User_ID,Create_Time,Auditor_ID,Audit_Time,'"+"取消待发送任务"+"',"+0+",sysdate,Send_Plan,Send_Already,Org_ID from T_Info_Send_Task where Task_ID="+sendTask.TASK_ID+"";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
			}
		    sql = "delete T_Info_Send_Task where Task_ID = "+sendTask.TASK_ID+"";
		    factory.excute(sql);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
			}
			factory.closeTransaction();
			 reponse = new FlexReponse(request,0,"<"+sendTask.TASK_NAME+">任务已取消!");
		}
		if(sendTask.STATUS.equals("4")){
			sql = "insert into T_Info_Send_Task_His(Task_ID,Task_Name,Content,Send_Group_ID,Language,Info_Type,Start_Time,End_Time,Pay_Type,Priority,Return_Report,Flow_Rate,User_ID,Create_Time,Auditor_ID,Audit_Time,Record_Cause,Statistics_Flag,Insert_Time,Send_Plan,Send_Already,Org_ID)" +
					"select Task_ID,Task_Name,Content,Send_Group_ID,Language,Info_Type,Start_Time,End_Time,Pay_Type,Priority,Return_Report,Flow_Rate,User_ID,Create_Time,Auditor_ID,Audit_Time,'"+"挂起后取消发布"+"',"+0+",sysdate,Send_Plan,Send_Already,Org_ID from T_Info_Send_Task where Task_ID="+sendTask.TASK_ID+"";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
			}
		    sql = "delete T_Info_Send_Task where Task_ID = "+sendTask.TASK_ID+"";
		    factory.excute(sql);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
			}
			factory.closeTransaction();
			 reponse = new FlexReponse(request,0,"<"+sendTask.TASK_NAME+">任务已取消!");
		}
		if(sendTask.STATUS.equals("5")){
			sql = "update T_Info_Send_Of_Day set Amount_Of_Day = Amount_Of_Day + (select nvl(Send_Already,0) from T_Info_Send_Task where Task_ID = "+sendTask.TASK_ID+") where Org_ID = '"+sendTask.ORG_ID+"'";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
			}
			sql = "update T_Info_Total_Rate set Update_Time = sysdate, Total_Rate = Total_Rate - (select nvl(Flow_Rate,0) from T_Info_Send_Task where Task_ID = "+sendTask.TASK_ID+")";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
			}
			sql = "insert into T_Info_Send_Task_His(Task_ID,Task_Name,Content,Send_Group_ID,Language,Info_Type,Start_Time,End_Time,Pay_Type,Priority,Return_Report,Flow_Rate,User_ID,Create_Time,Auditor_ID,Audit_Time,Record_Cause,Statistics_Flag,Insert_Time,Send_Plan,Send_Already,Org_ID)" +
					"select Task_ID,Task_Name,Content,Send_Group_ID,Language,Info_Type,Start_Time,End_Time,Pay_Type,Priority,Return_Report,Flow_Rate,User_ID,Create_Time,Auditor_ID,Audit_Time,'"+"暂停后取消发布"+"',"+0+",sysdate,Send_Plan,Send_Already,Org_ID from T_Info_Send_Task where Task_ID="+sendTask.TASK_ID+"";
			factory.excute(sql);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
			}
		    sql = "delete T_Info_Send_Task where Task_ID = "+sendTask.TASK_ID+"";
		    factory.excute(sql);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
			}

			factory.closeTransaction();
			 reponse = new FlexReponse(request,0,"<"+sendTask.TASK_NAME+">任务已取消!");
		}
		
		return reponse;
	}
	private FlexReponse submitT_Info_Send_Task(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("delSendTask");
		String sql = "";
	    sql = "update T_Info_Send_Task set Status ='1' where Task_ID = "+sendTask.TASK_ID+"";
		DBBuilder builder = new DBBuilder();
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"<"+sendTask.TASK_NAME+">任务已提交进入待审核状态!");
		return reponse;
	}
	private FlexReponse suspensionT_Info_Send_Task(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("delSendTask");
		String sql = "";
	    sql = "update T_Info_Send_Task set Status ='4' where Task_ID = "+sendTask.TASK_ID+"";
		DBBuilder builder = new DBBuilder();
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"<"+sendTask.TASK_NAME+">任务已经被挂起!");
		return reponse;
	}
	private FlexReponse CancelsSuspensionT_Info_Send_Task(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("delSendTask");
		String sql = "";
	    sql = "update T_Info_Send_Task set Status ='3' where Task_ID = "+sendTask.TASK_ID+"";
		DBBuilder builder = new DBBuilder();
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"<"+sendTask.TASK_NAME+">任务已经进入待发送状态!");
		return reponse;
	}
	private FlexReponse updateT_Info_Send_TaskAll(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("sendTask");
		ParamHandler handler = new ParamHandler();
		T_INFO_NUMBER_OBJECT sendObject = new T_INFO_NUMBER_OBJECT();
		List listSendObject = handler.queryT_Info_Number_Object(sendTask.SEND_GROUP_ID);
		String sql = "";
		if(listSendObject.size() < 1){
			throw new Exception("查询发送对象失败，请重新修改！");
		}else{
			sendObject = (T_INFO_NUMBER_OBJECT)listSendObject.get(0);
		}
//		System.out.println(sendTask.STATUS+"-------------------------------------------------00000000000000000000000");
			 sql = "update T_Info_Send_Task set Task_Name='"+sendTask.TASK_NAME+"',Content='"+sendTask.CONTENT+"',Send_Group_ID="+sendTask.SEND_GROUP_ID+",Language='"+sendObject.LANGUAGE+"'" +
					",Info_Type='"+sendTask.INFO_TYPE+"',Start_Time=to_date('"+sendTask.START_TIME+"','yyyy-mm-dd hh24:mi:ss'),End_Time=to_date('"+sendTask.END_TIME+"','yyyy-mm-dd hh24:mi:ss'),Status='0'" +
					",Send_Plan="+sendObject.AMOUNT+" where Task_ID ='"+sendTask.TASK_ID+"'";

		DBBuilder builder = new DBBuilder();
		
		builder.excute(sql);
		if(builder.SQL_Error_Code == 1){
			throw new Exception("任务名称已存在，请重新录入！");
		}
		if(builder.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"任务修改成功!");
		return reponse;
	}
	private FlexReponse stopSuspensionT_Info_Send_Task(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("delSendTask");
		String sql = "";
	    sql = "update T_Info_Send_Task set Status ='5' where Task_ID = "+sendTask.TASK_ID+"";
		DBBuilder builder = new DBBuilder();
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"<"+sendTask.TASK_NAME+">任务已经被暂停!");
		return reponse;
	}
	private FlexReponse continueSuspensionT_Info_Send_Task(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("delSendTask");
		String sql = "";
	    sql = "update T_Info_Send_Task set Status ='6' where Task_ID = "+sendTask.TASK_ID+"";
		DBBuilder builder = new DBBuilder();
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"<"+sendTask.TASK_NAME+">任务正在发送中!");
		return reponse;
	}
	private FlexReponse queryT_Info_Send_Task_HIS(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK_HIS sendTask  = (T_INFO_SEND_TASK_HIS)request.getBean("sendTask");
		String sql = "";
		DBBuilder builder = new DBBuilder();
		sql = "select t.Task_ID Task_ID,t.Task_Name Task_Name,t.Content Content,t.Start_Time Start_Time,t.End_Time End_Time," +
				" t.Send_Plan Send_Plan,r.Send_Suc Send_Suc,(t.Send_Plan-r.Send_Suc) SEND_FAULT,t.Insert_Time Insert_Time,t.Org_ID Org_ID,t.Record_Cause Record_Cause,t.Return_Cause Return_Cause " +
				" from T_Info_Send_Task_His t,TR_Info_Send_Result r"+
				" where r.Task_ID = t.Task_ID and " +
				" to_char(t.Create_Time,'yyyy-mm-dd hh24:mi:ss')>='"+sendTask.START_TIME+"' and to_char(t.Create_Time,'yyyy-mm-dd hh24:mi:ss')<='"+sendTask.END_TIME+"'";
//		 and Org_ID='"+sendTask.ORG_ID+"' 
		if(sendTask.CREATE_TIME != "" && sendTask.CREATE_TIME.length()>0 && sendTask.INSERT_TIME != "" && sendTask.INSERT_TIME.length()>0){
			sql+=" and to_char(t.Start_Time,'yyyy-mm-dd hh24:mi:ss')>='"+sendTask.CREATE_TIME+"' and to_char(t.Start_Time,'yyyy-mm-dd hh24:mi:ss')<='"+sendTask.INSERT_TIME+"'";
		}
		if(sendTask.TASK_NAME != "" && sendTask.TASK_NAME.length()>0){
			sql+=" and t.Task_Name like '%"+sendTask.TASK_NAME+"%'";
		}
		if(sendTask.RECORD_CAUSE != "" && sendTask.RECORD_CAUSE.length()>0){
			sql+=" and t.Record_Cause like '%"+sendTask.RECORD_CAUSE+"%'";
		}
		if(sendTask.INFO_TYPE != "" && sendTask.INFO_TYPE.length()>0){
			sql+=" and t.Info_Type = '"+sendTask.INFO_TYPE+"'";
		}
		if(sendTask.ORG_ID != "" && sendTask.ORG_ID.length()>0){
			sql+= " and t.Org_ID='"+sendTask.ORG_ID+"'" ;
		}
		if(sendTask.USER_ID != "" && sendTask.USER_ID.length()>0){
			sql+= " and t.User_ID='"+sendTask.USER_ID+"'" ;
		}
		if(sendTask.LANGUAGE != "" && sendTask.LANGUAGE.length()>0){
			sql+= " and t.Org_ID='"+sendTask.LANGUAGE+"'" ;
		}
		sql+=" order by Start_Time desc";
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"");
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryTInfoType_ORGNAME(FlexRequest request) throws Exception{
		ParamHandler builder = new ParamHandler();
		List<Map<String,Object>> list  = builder.getT_Info_TypeAll("");
		List<Map<String,Object>> list1  = builder.getT_OrgID__OrgNameAll("");
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		reponse.set("list1", list1);
		return reponse;
	}
	private FlexReponse takeHotelTiminerPhoneNumber(FlexRequest request) throws Exception{
		T_INFO_NUMBER_OBJECT sendObject = (T_INFO_NUMBER_OBJECT)request.getBean("sendObject");
		ArrayList hotelLevelList = (ArrayList)request.getBean("arrHotelLevel");
		ArrayList hotelNameList = (ArrayList)request.getBean("arrHotelName");
		String Hotel_Grade_data = "";
		int count = 0;
		FlexReponse reponse = null;
		List<Map<String,Object>> listNewLevel = new  ArrayList<Map<String,Object>>();
		if(hotelLevelList.size()>0){
			ParamHandler builder = new ParamHandler();
			listNewLevel  = builder.getT_Org_InfoHoteName(hotelLevelList);
		}
		if(listNewLevel.size()>0){
			for(int i = 0;i<listNewLevel.size();i++){
				Map<String,Object> map = (Map<String,Object>)listNewLevel.get(i);	
				Hotel_Grade_data +="'"+map.get("data")+"',";
			}
		}
		if(hotelNameList.size()>0){
			for(int i = 0;i<hotelNameList.size();i++){
				Map map = (Map)hotelNameList.get(i);	
				Hotel_Grade_data +="'"+map.get("data")+"',";
			}
		}
		if(Hotel_Grade_data.length() > 0){
			Hotel_Grade_data =  Hotel_Grade_data.substring(0, Hotel_Grade_data.length()-1);
		}else{
			Hotel_Grade_data = "''";
		}
		DBBuilder builder = new DBBuilder();
		count = builder.getCount("T_Terminal_Info", "Status='12' and Language='"+sendObject.LANGUAGE+"' and Hotel_ID in("+Hotel_Grade_data+")");
		List<Map<String,Object>> list = builder.queryList("select Msisdn from T_Terminal_Info where Status='12' and Language='"+sendObject.LANGUAGE+"' and Hotel_ID in("+Hotel_Grade_data+") ");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		if(count>0){
			reponse = new FlexReponse(request,0,"根据您的条件共找到"+count+"个终端手机号码");
		}else{
			reponse = new FlexReponse(request,0,"没有查询到终端手机号码，请确认！");
		}
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse verificationT_Info_Send_Task(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("delSendTask");
		String sql = "";
	    sql = "update T_Info_Send_Task set Status ='2' where Task_ID = "+sendTask.TASK_ID+"";
		DBBuilder builder = new DBBuilder();
		builder.excute(sql);

		if(builder.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"<"+sendTask.TASK_NAME+">任务正在审核!");
		return reponse;
	}
	private FlexReponse verificationPassesT_Info_Send_Task(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("sendTask");
		String sql = "";
	    sql = "update T_Info_Send_Task set Status ='3',Priority="+sendTask.PRIORITY+",Return_Report="+sendTask.RETURN_REPORT+",Flow_Rate="+sendTask.FLOW_RATE+",Auditor_ID='"+sendTask.AUDITOR_ID+"',Audit_Time=sysdate where Task_ID = "+sendTask.TASK_ID;
		DBBuilder builder = new DBBuilder();
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"<"+sendTask.TASK_NAME+">任务已通过审核，进入等待发送状态!");
		return reponse;
	}
	private FlexReponse verificationBackT_Info_Send_Task(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("sendTask");
		String sql = "";
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		sql = "insert into T_Info_Send_Task_His(Task_ID,Task_Name,Content,Send_Group_ID,Language,Info_Type,Start_Time,End_Time,Pay_Type,Priority,Return_Report,Flow_Rate,User_ID,Create_Time,Auditor_ID,Audit_Time,Record_Cause,Statistics_Flag,Insert_Time,Send_Plan,Send_Already,Org_ID,Return_Cause)" +
		"select Task_ID,Task_Name,Content,Send_Group_ID,Language,Info_Type,Start_Time,End_Time,Pay_Type,Priority,Return_Report,Flow_Rate,User_ID,Create_Time,'"+sendTask.AUDITOR_ID+"',sysdate,'"+"被打回"+"',"+0+",sysdate,Send_Plan,Send_Already,Org_ID,'"+sendTask.RETURN_REPORT+"' from T_Info_Send_Task where Task_ID="+sendTask.TASK_ID+"";
		factory.excute(sql);
		if(factory.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
		}
		sql = "delete T_Info_Send_Task where Task_ID = "+sendTask.TASK_ID+"";
		factory.excute(sql);
		if(factory.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+factory.SQL_Error_Code+" 错误信息:"+factory.SQL_Error_Message);
		}
		factory.closeTransaction();
		FlexReponse reponse = new FlexReponse(request,0,"<"+sendTask.TASK_NAME+">任务已被打回!");
		return reponse;
	}
	private FlexReponse verificationCancelT_Info_Send_Task(FlexRequest request) throws Exception{
		T_INFO_SEND_TASK sendTask  = (T_INFO_SEND_TASK)request.getBean("sendTask");
		String sql = "";
	    sql = "update T_Info_Send_Task set Status ='1' where Task_ID = "+sendTask.TASK_ID+"";
		DBBuilder builder = new DBBuilder();
		System.out.println(sql);
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("错误代码:"+builder.SQL_Error_Code+" 错误信息:"+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"<"+sendTask.TASK_NAME+">任务已取消审核，正处在待审核状态!");
		return reponse;
	}
}
