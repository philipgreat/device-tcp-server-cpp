package cn.ucans.flex.services;

import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import jxl.Workbook;


import chobits.db.DBBuilder;
import chobits.init.ChobitsConfig;
import chobits.log.Logger;
import cn.ucans.bean.TR_TERMINAL_HIRED;
import cn.ucans.bean.TR_TERMINAL_IN_OUT;
import cn.ucans.bean.T_ACCESS_LOG;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class HotelTerminalReportService extends FlexService{
	
	private Logger logger = new Logger(HotelTerminalReportService.class.getName());
	String strFileData = "";
	String strStyle = "";
	String strFileName = "";
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------酒店终端报表 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 2040:
					reponse = this.queryHotelName(request);
					break;
				case 2041:
					reponse = this.queryTR_Terminal_Hired(request);
					break;
				case 2044:
					reponse = this.queryTR_Terminal_Use(request);
					break;
				case 2045:
					reponse = this.queryHotelNameAllSimple(request);
					break;
				case 2046:
					reponse = this.queryTR_Terminal_In_Out(request);
					break;
				case 2047:
					reponse = this.queryT_ACCESS_LOG(request);
					break;
				case 2048:
					reponse = this.queryHotelNameLanguageAll(request);
					break;
				case 2049:
					reponse = this.queryT_ACCESS_LOG_TOP10(request);
					break;
				default:
					break;
			}
			logger.debug("------------------------------酒店终端报表任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("------------------------------酒店终端报表任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	private FlexReponse queryT_ACCESS_LOG_TOP10(FlexRequest request) throws Exception{
		T_ACCESS_LOG termenal = (T_ACCESS_LOG)request.getBean("termenal");
		String sql = "";
		sql = "Select  Content_Prefix, Language, Content_Path,count(*) Click_Count" +
				" From T_Access_Log" +
				" Where to_char(Create_Time,'yyyy-mm-dd')>='"+termenal.CREATE_TIME+"' and" +
				" to_char(Create_Time,'yyyy-mm-dd')<='"+termenal.MSISDN+"'";
		if(termenal.HOTEL_ID.length() > 0){
			sql += " and Hotel_id = '"+termenal.HOTEL_ID+"'";
		}
		if(termenal.IMEI_NUMBER.length() > 0){
			sql += " and Hotel_id in(select Org_ID from T_Org_Info where Org_Type='3' and Superior_Org_ID = '"+termenal.IMEI_NUMBER+"')";
		}
		if(termenal.LANGUAGE.length() > 0){
			sql += " and Language = '"+termenal.LANGUAGE+"'";
		}
		if(termenal.CONTENT_PATH.length() > 0){
			sql += " and Content_Path like '%"+termenal.CONTENT_PATH+"%'";
		}
		sql += " Group by Content_Prefix,Language,Content_Path Order by count(*) desc";
		String sql1 = "select rownum nameCount ,Content_Prefix,Language,Content_Path,Click_Count from ("+sql+") where rownum<=10";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(sql1);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("获取信息时出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryHotelNameLanguageAll(FlexRequest request) throws Exception{
		ParamHandler builder = new ParamHandler();
		Map<String,Object> map = new HashMap<String,Object>();
		map.put("label", "全部");
		map.put("data", "");
		List<Map<String,Object>> list  = builder.get_T_Org_Info_NoStatus("");
		List<Map<String,Object>> list1  = builder.getT_Org_Info_Org_ID_Org_Name_NoStatus("");
		List<Map<String,Object>> list2  = builder.getLanguageName("");
		list1.add(0,map);
		list2.add(0,map);
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		reponse.set("list1", list1);
		reponse.set("list2", list2);
		return reponse;
	}
	private FlexReponse queryT_ACCESS_LOG(FlexRequest request) throws Exception{
		T_ACCESS_LOG termenal = (T_ACCESS_LOG)request.getBean("termenal");
		String sql = "";
		sql = "Select to_char(a.Create_Time,'yyyy-mm-dd') Create_Time,b.Org_Name Org_Name,c.Org_Name BROKER_ID,a.Content_Prefix Content_Prefix,a.Language Language,a.Content_Path Content_Path,count(*) Click_Count" +
				" From T_Access_Log a,T_Org_Info b,T_Org_Info c" +
				" Where a.Hotel_ID=b.Org_ID and b.Superior_Org_ID=c.Org_ID and " +
				" to_char(a.Create_Time,'yyyy-mm-dd')>='"+termenal.CREATE_TIME+"' and" +
				" to_char(a.Create_Time,'yyyy-mm-dd')<='"+termenal.MSISDN+"'";
		if(termenal.HOTEL_ID.length() > 0){
			sql += " and a.Hotel_ID = '"+termenal.HOTEL_ID+"'";
		}
		if(termenal.IMEI_NUMBER.length() > 0){
			sql += " and b.Superior_Org_ID = '"+termenal.IMEI_NUMBER+"'";
		}
		sql += " Group by a.Create_Time,b.Org_Name,c.Org_Name," +
				" a.Content_Prefix,a.Language,a.Content_Path order by Create_Time desc";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("获取信息时出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryTR_Terminal_In_Out(FlexRequest request) throws Exception{
		TR_TERMINAL_IN_OUT inOut = (TR_TERMINAL_IN_OUT)request.getBean("inOut");
		String sql = "";
		sql = "Select Statis_Date Statis_Date," +
				"sum(Terminal_Count*decode(Change_Type,'01',1,0)) insertCount," +
				"sum(Terminal_Count*decode(Change_Type,'02',1,0)) toHotelCount," +
				"sum(Terminal_Count*decode(Change_Type,'03',1,0)) fromHotelCount," +
				"sum(Terminal_Count*decode(Change_Type,'04',1,0)) badCount," +
				"sum(Terminal_Count*decode(Change_Type,'00',1,0)) allCount" +
				" From TR_Terminal_In_Out" +
				" Where Statis_Date>='"+inOut.STATIS_DATE+"' and Statis_Date<='"+inOut.STATIS_TIME+"'";
		if(inOut.HOTEL_ID.length() >0){
			sql += " and Hotel_ID='"+inOut.HOTEL_ID+"' Group by Statis_Date order by Statis_Date desc";
		}else{
			sql += " Group by Statis_Date order by Statis_Date desc";
		}
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list  = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("获取信息时出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryHotelNameAllSimple(FlexRequest request) throws Exception{
		ParamHandler builder = new ParamHandler();
		List<Map<String,Object>> list  = builder.get_T_Org_Info_NoStatus("");
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryTR_Terminal_Use(FlexRequest request) throws Exception{
		TR_TERMINAL_HIRED termenal = (TR_TERMINAL_HIRED)request.getBean("termenal");
//		int broketId = request.type;
		String sql = "";
		sql = "Select a.Statis_Date Statis_Date,c.Org_Name Org_Name,d.Org_Name Broker_ID,b.Terminal_Count Terminal_Count,a.Hire_Seconds/3600 hire_Time,(100*a.Hire_Seconds/3600/24/b.Terminal_Count)||'%' as usePercentage" +
				" From TR_Terminal_Hired a,TR_Terminal_In_Out b,T_Org_Info c,T_Org_Info d " +
				" Where a.Statis_Date>='"+termenal.STATIS_DATE+"' and a.Statis_Date<='"+termenal.STATIS_TIME+"' and a.Hire_Seconds>0 and a.Statis_Date=b.Statis_Date and b.Change_Type='00' and a.Hotel_ID=b.Hotel_ID and a.Hotel_ID=c.Org_ID and a.Broker_ID=d.Org_ID  ";
		if(termenal.HOTEL_ID.length() > 0){
			sql += " and a.Hotel_ID = '"+termenal.HOTEL_ID+"'";
		}
		if(termenal.BROKER_ID.length() > 0){
			sql += " and a.Broker_ID = '"+termenal.BROKER_ID+"'";
		}
		sql += " order by Statis_Date desc";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("获取信息时出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryTR_Terminal_Hired(FlexRequest request) throws Exception{
		TR_TERMINAL_HIRED termenal = (TR_TERMINAL_HIRED)request.getBean("termenal");
		String sql = "";
//		int broketId = request.type;
//		if(broketId == 1){
//			sql = "Select d.Org_Name Broker_ID,a.Statis_Date Statis_Date,b.Terminal_Count Terminal_Count,a.Hire_Count Hire_Count,(100*a.Hire_Count/b.Terminal_Count)||'%' as Percentage  From" +
//			" TR_Terminal_Hired a,TR_Terminal_In_Out b,T_Org_Info c,T_Org_Info d " +
//			" Where a.Statis_Date>='"+termenal.STATIS_DATE+"' and a.Statis_Date<='"+termenal.STATIS_TIME+"' and a.Hire_Count>0 and a.Statis_Date=b.Statis_Date and b.Change_Type='00' and a.Hotel_ID=b.Hotel_ID and a.Hotel_ID=c.Org_ID  and a.Broker_ID=d.Org_ID";
//		}else{
			sql = "Select a.Statis_Date Statis_Date,c.Org_Name Org_Name,d.Org_Name Broker_ID,b.Terminal_Count Terminal_Count,a.Hire_Count Hire_Count,(100*a.Hire_Count/b.Terminal_Count)||'%' as Percentage  From" +
					" TR_Terminal_Hired a,TR_Terminal_In_Out b,T_Org_Info c,T_Org_Info d " +
					" Where a.Statis_Date>='"+termenal.STATIS_DATE+"' and a.Statis_Date<='"+termenal.STATIS_TIME+"' and a.Hire_Count>0 and a.Statis_Date=b.Statis_Date and b.Change_Type='00' and a.Hotel_ID=b.Hotel_ID and a.Hotel_ID=c.Org_ID  and a.Broker_ID=d.Org_ID";
//		}
		if(termenal.HOTEL_ID.length() > 0){
			sql += " and a.Hotel_ID = '"+termenal.HOTEL_ID+"'";
		}
		if(termenal.BROKER_ID.length() > 0){
			sql += " and a.Broker_ID = '"+termenal.BROKER_ID+"'";
		}
		sql += " order by Statis_Date desc";
//		if(broketId == 1){
//			sql +=" group by Broker_ID,Statis_Date,Terminal_Count,Hire_Count,Percentage";
//		}
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(sql);
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryHotelName(FlexRequest request) throws Exception{
		ParamHandler builder = new ParamHandler();
		Map<String,Object> map = new HashMap<String,Object>();
		map.put("label", "全部");
		map.put("data", "");
		List<Map<String,Object>> list  = builder.get_T_Org_Info_NoStatus("");
		List<Map<String,Object>> list1  = builder.getT_Org_Info_Org_ID_Org_Name_NoStatus("");
		list1.add(0,map);
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		reponse.set("list1", list1);
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
//		String	strLine;
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
