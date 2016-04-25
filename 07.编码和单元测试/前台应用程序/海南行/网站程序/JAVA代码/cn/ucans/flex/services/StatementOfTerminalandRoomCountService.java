package cn.ucans.flex.services;

import java.io.File;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import jxl.Workbook;


import chobits.db.DBBuilder;
import chobits.init.ChobitsConfig;
import chobits.log.Logger;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class StatementOfTerminalandRoomCountService extends FlexService{
	
	private Logger logger = new Logger(TerminalTypeService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------表单酒店房间数、终端数管理任务 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 9090:
					reponse = this.initStatement(request);
					break;
				case 9091:
					reponse = this.QueryStatement(request);
					break;
				case 9092:
					break;
				case 9093:
					break;
				case 9094:
					break;
				case 9095:
					break;
				case 9096:
					break;
				default:
					break;
			}
			logger.debug("-----------------------------表单酒店房间数、终端数管理任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------表单酒店房间数、终端数管理任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initStatement(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		
		String orgType=(String)builder.queryObjectClumn("select Org_Type from T_Org_Info where Org_ID='"+request.org_id+"'");
		String sql="";
		
		
		if("0".equals(orgType)){
			sql="Select a.Org_ID,a.Org_Name ORG_HOTEL_NAME,b.Org_Name ORG_AGENT_NAME,a.Room_Count,count(*) TERMINAL_COUNT" +
			" From T_Org_Info a,T_Org_Info b,T_Terminal_Info c " +
			" Where a.Org_Type='3' and a.Superior_Org_ID=b.Org_ID and b.Org_Type='2' and a.Org_ID=c.Hotel_ID " +
			" Group by a.Org_ID,a.Org_Name,b.Org_Name,a.Room_Count";
		}else if("3".equals(orgType)){
			sql="Select a.Org_ID,a.Org_Name ORG_HOTEL_NAME,a.Room_Count,count(*) TERMINAL_COUNT From T_Org_Info a,T_Terminal_Info b " +
					" Where a.Org_Type='3' and a.Org_ID=b.Hotel_ID and a.Org_ID='"+request.org_id+"'" 
					+" Group by a.Org_ID,a.Org_Name,a.Room_Count";
		}else{
			throw new Exception("对不起，您无权限操作此菜单！！");
		}
		
		
		
		List<Map<String,Object>> list = builder.queryList(sql);
		
		if(builder.SQL_Error_Code != 0){		
			throw new Exception("查询表单酒店房间数、终端数信息，获取表单酒店房间数、终端数出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		List<Map<String,Object>> queryOrgNameList = null;
		List<Map<String,Object>> listForOrgName =null;
		if("0".equals(orgType)){
			queryOrgNameList = new ParamHandler().getT_OrgID__OrgName("3");
			
			if(builder.SQL_Error_Code != 0){			
				throw new Exception("查询表单酒店房间数、终端数信息，获取表单酒店房间数、终端数出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}
			
			listForOrgName = new LinkedList<Map<String,Object>>();
			Map<String,Object> queryAllOrgListMap=new HashMap<String,Object>();
			queryAllOrgListMap.put("label", "全部");
			queryAllOrgListMap.put("data", "");
			listForOrgName.add(queryAllOrgListMap);
			for(int i=0;i<queryOrgNameList.size();i++){
				listForOrgName.add(queryOrgNameList.get(i));
			}
			
			
		}
		
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		reponse.set("orgType", orgType);
		reponse.set("listForOrgName", listForOrgName);
		return reponse;
	}
	
	private FlexReponse QueryStatement(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		String sql="";
		String orgId=(String)request.getBean("orgId");
		
		if("".equals(orgId)){
			sql="Select a.Org_ID,a.Org_Name ORG_HOTEL_NAME,b.Org_Name ORG_AGENT_NAME,a.Room_Count,count(*) TERMINAL_COUNT" +
			" From T_Org_Info a,T_Org_Info b,T_Terminal_Info c " +
			" Where a.Org_Type='3' and a.Superior_Org_ID=b.Org_ID and b.Org_Type='2' and a.Org_ID=c.Hotel_ID " +
			" Group by a.Org_ID,a.Org_Name,b.Org_Name,a.Room_Count";
		}else{
			sql="Select a.Org_ID,a.Org_Name ORG_HOTEL_NAME,b.Org_Name ORG_AGENT_NAME,a.Room_Count,count(*) TERMINAL_COUNT" +
			" From T_Org_Info a,T_Org_Info b,T_Terminal_Info c " +
			" Where a.Org_Type='3' and a.Superior_Org_ID=b.Org_ID and b.Org_Type='2' and a.Org_ID=c.Hotel_ID and a.Org_Id='" +orgId +"'"+
			" Group by a.Org_ID,a.Org_Name,b.Org_Name,a.Room_Count";
			
		}
		
		List<Map<String,Object>> list = builder.queryList(sql);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("查询表单酒店房间数、终端数信息，获取表单酒店房间数、终端数出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
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
