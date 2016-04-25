package cn.ucans.flex.services;

import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import jxl.Workbook;
import chobits.db.DBBuilder;
import chobits.db.DBFactory;
import chobits.init.ChobitsConfig;
import chobits.log.Logger;
import cn.ucans.bean.T_ACCESS_LOG;
import cn.ucans.bean.T_SERVICE_CASH;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler; 

public class CashReportManagerFlexService extends FlexService{
	
	private Logger logger = new Logger(CashReportManagerFlexService.class.getName());
	String strFileData = "";
	String strStyle = "";
	String strFileName = "";
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------帐号报表任务 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 2005:
					reponse = this.CreateExcelFile(strFileData,strStyle,request);
					break;
				case 2006:
					reponse = this.DeleteExcelFile(strFileName, request);
					break;
				case 2050:
					reponse = this.queryHotelNameLanguageAll(request);
					break;
				case 2051:
					reponse = this.queryT_Service_Cash(request);
					break;
				case 2052:
					reponse = this.queryT_Service_Cash_Month(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------帐号报表任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------帐号报表任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	private FlexReponse queryT_Service_Cash_Month(FlexRequest request) throws Exception{
		T_SERVICE_CASH cashReport = (T_SERVICE_CASH)request.getBean("cashReport");
		String sql = "";
		String sql1 = "";
		String startTime = cashReport.START_TIME;
		
		sql = "Select Oper_User Oper_User,to_char(sum(Real_Pay_Cash/100)) Real_Pay_Cash,to_char(sum(Cash/100)) Cash,to_char(sum(Free_Cash/100)) Free_Cash " +
				" From V_Service_Cash" +
				" Where to_char(Create_Time,'yyyy-mm-dd')='"+cashReport.START_TIME+"' and Hotel_ID='"+cashReport.HOTEL_ID+"' and status='1' ";
	
		sql1 = "Select '总计:' as Oper_User,to_char(sum(Real_Pay_Cash/100)) Real_Pay_Cash,to_char(sum(Cash/100)) Cash,to_char(sum(Free_Cash/100)) Free_Cash " +
				" From V_Service_Cash" +
				" Where to_char(Create_Time,'yyyy-mm-dd')='"+cashReport.START_TIME+"' and Hotel_ID='"+cashReport.HOTEL_ID+"' and status='1' ";
		sql += " Group by Oper_User";
		sql1 += " Group by '总计:'";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("获取信息时出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		if(list.size() >0){
			List<Map<String,Object>> list1 = builder.queryList(sql1);
			Map<String,Object> map = (Map<String,Object>)list1.get(0);
			list.add(map);
		}
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("获取信息时出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryT_Service_Cash(FlexRequest request) throws Exception{
		T_SERVICE_CASH cashReport = (T_SERVICE_CASH)request.getBean("cashReport");
		String sql = "";
		String sql1 = "";
//		String startTime = cashReport.START_TIME.substring(0, cashReport.START_TIME.length()-3);
//		String endTime = cashReport.END_TIME.substring(0, cashReport.END_TIME.length()-3);
		String startTime = cashReport.START_TIME;
		String endTime = cashReport.END_TIME;
		
		sql = "Select Oper_User Oper_User,to_char(sum(Real_Pay_Cash/100)) Real_Pay_Cash,to_char(sum(Cash/100)) Cash,to_char(sum(Free_Cash/100)) Free_Cash " +
				" From V_Service_Cash" +
				" Where to_char(Create_Time,'yyyy-mm-dd')>='"+cashReport.START_TIME+"' and" +
				" to_char(Create_Time,'yyyy-mm-dd')<='"+cashReport.END_TIME+"' and Hotel_ID='"+cashReport.HOTEL_ID+"' and status='1' ";
		sql1 = "Select '总计:' as Oper_User,to_char(sum(Real_Pay_Cash/100)) Real_Pay_Cash,to_char(sum(Cash/100)) Cash,to_char(sum(Free_Cash/100)) Free_Cash " +
				" From V_Service_Cash" +
				" Where to_char(Create_Time,'yyyy-mm-dd')>='"+cashReport.START_TIME+"' and" +
				" to_char(Create_Time,'yyyy-mm-dd')<='"+cashReport.END_TIME+"' and Hotel_ID='"+cashReport.HOTEL_ID+"' and status='1' ";
		sql += " Group by Oper_User";
		sql1 += " Group by '总计:'";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("获取信息时出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		if(list.size() >0){
			List<Map<String,Object>> list1 = builder.queryList(sql1);
			Map<String,Object> map = (Map<String,Object>)list1.get(0);
			list.add(map);
		}
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("获取信息时出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryHotelNameLanguageAll(FlexRequest request) throws Exception{
		ParamHandler builder = new ParamHandler();
		DBBuilder builder1 = new DBBuilder(); 
		Map<String,Object> map = new HashMap<String,Object>();
		map.put("label", "全部");
		map.put("data", "");
		List<Map<String,Object>> list  = builder.getT_User_InfoAllWhere(request.org_id);
		List<Map<String,Object>> list1  = builder.get_T_Org_Info_NoStatusWhere(request.org_id);
		list.add(0,map);
		String name = (String)builder1.queryObjectClumn("select User_Name from T_User_Info where User_ID='"+request.user_id+"'");
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		reponse.set("list1", list1);
		reponse.set("name", name);
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
			jxl.write.WritableSheet ws = wwb.createSheet("报表导出数据", 0);
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
	
}
