package cn.ucans.flex.services;

import java.io.File;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import jxl.Workbook;


import chobits.db.DBBuilder;
import chobits.init.ChobitsConfig;
import chobits.log.Logger;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;

public class StatementOfDayForPersonService extends FlexService{
	
	private Logger logger = new Logger(TerminalTypeService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------人员日报表管理任务 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 9130:
					reponse = this.initStatement(request);
					break;
				case 9131:
					reponse = this.QueryStatement(request);
					break;
				case 9132:
					break;
				case 9133:
					break;
				case 9134:
					break;
				case 9135:
					break;
				case 9136:
					break;
				default:
					break;
			}
			logger.debug("-----------------------------人员日报表管理任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------人员日报表管理任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initStatement(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		
		String time=(String)request.getBean("time");
		
		String sql="select Create_Time,sum(Free_Cash)/100 FREE_CASH,sum(Real_Pay_Cash)/100 REAL_PAY_CASH  from V_SERVICE_CASH where Status='1' and OPER_USER='"
			
			+request.user_id+"' and to_char(Create_Time,'yyyy-mm-dd')='"+time+"'  group by Create_Time";
		
		List<Map<String,Object>> list = builder.queryList(sql);
		
		if(builder.SQL_Error_Code != 0){		
			throw new Exception("查询人员日报表信息，获取人员日报表出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		String sqlForOrgName = "select Org_Name from T_Org_Info where Org_ID='"+request.org_id+"'";
		
		String orgName = (String)builder.queryObjectClumn(sqlForOrgName);
		if(builder.SQL_Error_Code != 0){		
			throw new Exception("查询人员日报表信息，获取人员日报表出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		String sqlForUserName = "select User_Name from T_User_Info where User_ID='"+request.user_id+"'";
		
		String userName = (String)builder.queryObjectClumn(sqlForUserName);
		if(builder.SQL_Error_Code != 0){		
			throw new Exception("查询人员日报表信息，获取人员日报表出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		if(list.size()>0){
			String sql2="select sum(Free_Cash)/100 FREE_CASH,sum(Real_Pay_Cash)/100 REAL_PAY_CASH  from V_SERVICE_CASH where Status='1' and OPER_USER='"
				
				+request.user_id+"' and to_char(Create_Time,'yyyy-mm-dd')='"+time+"'";
			List<Map<String,Object>> list2 = builder.queryList(sql2);
			
			Iterator iter = list2.iterator();
			while(iter.hasNext()){
				Map<String,Object> map = (Map<String,Object>)iter.next();
				map.put("CREATE_TIME", "                              总计：");
				list.add(map);
			}
			
			
			if(builder.SQL_Error_Code != 0){		
				throw new Exception("查询人员日报表信息，获取人员日报表出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}
		}
		
		
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("orgName", orgName);
		reponse.set("userName", userName);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse QueryStatement(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		String time=(String)request.getBean("time");
		
		String sql="select Create_Time,sum(Free_Cash)/100 FREE_CASH,sum(Real_Pay_Cash)/100 REAL_PAY_CASH  from V_SERVICE_CASH where Status='1' and OPER_USER='"
			
			+request.user_id+"' and to_char(Create_Time,'yyyy-mm-dd')='"+time+"'  group by Create_Time";
		
		List<Map<String,Object>> list = builder.queryList(sql);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("查询人员日报表信息，获取人员日报表出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		if(list.size()>0){
			String sql2="select sum(Free_Cash)/100 FREE_CASH,sum(Real_Pay_Cash)/100 REAL_PAY_CASH  from V_SERVICE_CASH where Status='1' and OPER_USER='"
				
				+request.user_id+"' and to_char(Create_Time,'yyyy-mm-dd')='"+time+"'";
			List<Map<String,Object>> list2 = builder.queryList(sql2);
			
			Iterator iter = list2.iterator();
			while(iter.hasNext()){
				Map<String,Object> map = (Map<String,Object>)iter.next();
				map.put("CREATE_TIME", "                              总计：");
				list.add(map);
			}
			
			
			if(builder.SQL_Error_Code != 0){		
				throw new Exception("查询人员日报表信息，获取人员日报表出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}
		}
		
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		return reponse;
	}
	
	
	
	/**
	 * 导出数据为Excel表
	 * @param strFileData
	 * @param strStyle
	 * @param request
	 * @return
	 * @throws Exception
	 */
	public FlexReponse CreateExcelFile(String strFileData,String strStyle,FlexRequest request) throws Exception 
	{
		
		File	oFile = null;
		String	strLine;
		String	strCol;
		String	strData = strFileData;
		int		nLenPlace = 0;
		int		nColPlace;
//		System.out.println("------------------------------开始------------------------------");
//		System.out.println(strData);
//		System.out.println("==============================结束==============================");
		try 
		{
			File	oDir = new File(ChobitsConfig.TempFileForderPath);
//			File	oDir = new File("/Tomcat/webapps/CCSS/XLSTmepFile");
//			File	oDir = new File("F:\\workspace\\source\\Happy_Spase\\Happy\\WebRoot\\TempFile");
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
			reponse.set("fileName", fileName);
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
		try 
		{		
			java.io.File DelFile=new java.io.File(ChobitsConfig.TempFileForderPath+"/" + strFileName);
			//java.io.File DelFile=new java.io.File("F:\\workspace\\source\\Happy_Spase\\Happy\\WebRoot\\TempFile\\" + strFileName);
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
