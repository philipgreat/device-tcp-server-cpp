package cn.ucans.flex.services;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import jxl.Workbook;


import chobits.db.DBBuilder;
import chobits.init.ChobitsConfig;
import chobits.log.Logger;
import cn.ucans.bean.T_TERMINAL_SERVICE_POINT;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class TerminalServicePoint extends FlexService{
	
	private Logger logger = new Logger(TerminalTypeService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------终端生维修点管理任务 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 9010:
					reponse = this.initTerminalServicePoint(request);
					break;
				case 9011:
					reponse = this.addTerminalServicePoint(request);
					break;
				case 9012:
					reponse = this.queryTerminalServicePoint(request);
					break;
				case 9013:
					reponse = this.updateTerminalServicePoint(request);
					break;
				case 9014:
					reponse = this.deleteTerminalServicePoint(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------终端生维修点管理任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------终端生维修点管理任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initTerminalServicePoint(FlexRequest request) throws Exception{
		
		String select_TerminalServicePoint_Info="Select ID,Org_ID,Prov_Name,Area_Name,Tele_No,Zip_Code,Address From T_Terminal_Service_Point Order by Org_ID,Prov_Name,Area_Name";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_TerminalServicePoint_Info);
		
		List<Map<String,Object>> orgNameList = new ParamHandler().getT_OrgID__OrgName("1");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("查询终端维修点信息，获取终端维修点信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		List<Map<String,Object>> queryOrgNameList = new ArrayList<Map<String,Object>>();
		Map<String,Object> map  =new HashMap<String,Object>();
		map.put("label", "全部");
		map.put("data", "");
		queryOrgNameList.add(map);
		queryOrgNameList.addAll(orgNameList);
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		reponse.set("orgNameList", orgNameList);
		reponse.set("queryOrgNameList", queryOrgNameList);
		return reponse;
	}
	
	private FlexReponse queryTerminalServicePoint(FlexRequest request) throws Exception{
		T_TERMINAL_SERVICE_POINT terminalServicePoint = (T_TERMINAL_SERVICE_POINT) request.getBean("terminalServiePoint");
		String tempOrgId = terminalServicePoint.ORG_ID;
		String tempProv_Name = terminalServicePoint.PROV_NAME;
		String tempArea_Name = terminalServicePoint.AREA_NAME;
		
		String select_terminal_typeInfo="Select ID,Org_ID,Prov_Name,Area_Name,Tele_No,Zip_Code,Address From T_Terminal_Service_Point where 1=1";
		if(!tempOrgId.equals("")&&tempOrgId.length()>0){
			select_terminal_typeInfo +="  and Org_Id='"+tempOrgId+"'";
		}
		
		if(!"".equals(tempProv_Name)&&tempProv_Name.length()>0){
			select_terminal_typeInfo+=" and Prov_Name like '%"+tempProv_Name+"%'";
		}
		if(!"".equals(tempArea_Name)&&tempArea_Name.length()>0){		
			select_terminal_typeInfo+=" and Area_Name like '%"+tempArea_Name+"%'";
		}
		
		select_terminal_typeInfo+=" Order by Org_ID,Prov_Name,Area_Name";
		
		
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_terminal_typeInfo);;
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("查询终端维修点信息，获取终端维修点信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse addTerminalServicePoint(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_TERMINAL_SERVICE_POINT terminalServicePoint = (T_TERMINAL_SERVICE_POINT) request.getBean("terminalServicePoint");
		int count = builder.getCount("T_Terminal_Service_Point", " Org_ID='"+terminalServicePoint.ORG_ID+"' and Prov_Name='"+terminalServicePoint.PROV_NAME+"' and Area_Name='"+terminalServicePoint.AREA_NAME+"' and Tele_No='"+terminalServicePoint.TELE_NO+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("添加终端维修点，获取终端维修点信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count>0){
				throw new Exception("添加终端维修点，该终端维修点已经存在！");
			}
		}
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("添加终端维修点，获取终端维修点信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		String sql="Insert Into T_Terminal_Service_Point(ID,Org_ID,Prov_Name,Area_Name,Tele_No,Zip_Code,Address,Create_Time) Values(Five_Bit_SEQ.NEXTVAL,'"
				+terminalServicePoint.ORG_ID+ "','"+terminalServicePoint.PROV_NAME+"','" +terminalServicePoint.AREA_NAME+ "','"+terminalServicePoint.TELE_NO+"','"+terminalServicePoint.ZIP_CODE+"','"+terminalServicePoint.ADDRESS+"',sysdate)";
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("添加终端维修点出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"添加终端维修点信息成功！");
		return reponse;
	}
	
	private FlexReponse updateTerminalServicePoint(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_TERMINAL_SERVICE_POINT terminalServicePointForUpdate = (T_TERMINAL_SERVICE_POINT) request.getBean("terminalServicePointForUpdate");
		T_TERMINAL_SERVICE_POINT oldTerminalServicePointForUpdate = (T_TERMINAL_SERVICE_POINT) request.getBean("oldTerminalServicePointForUpdate");
		int count = builder.getCount("T_Terminal_Service_Point", "  ID="+oldTerminalServicePointForUpdate.ID+" and  Org_ID='"+oldTerminalServicePointForUpdate.ORG_ID+"' and Prov_Name='"+oldTerminalServicePointForUpdate.PROV_NAME+"' and Area_Name='"+oldTerminalServicePointForUpdate.AREA_NAME+"' and Tele_No='"+oldTerminalServicePointForUpdate.TELE_NO+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("修改终端维修点，获取终端维修点信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count==0){
				throw new Exception("对不起，该终端维修点已经不存在或已经被修改！请确认后在修改！");
			}
		}
		
		String sql = " update T_Terminal_Service_Point set Org_Id='"+terminalServicePointForUpdate.ORG_ID+"',Prov_Name='"+terminalServicePointForUpdate.PROV_NAME+"',Area_Name='"+terminalServicePointForUpdate.AREA_NAME+"',Tele_No='"+terminalServicePointForUpdate.TELE_NO+"',Zip_Code='"+terminalServicePointForUpdate.ZIP_CODE+"',Address='"+terminalServicePointForUpdate.ADDRESS+"',Last_Update_Time=sysdate where ID="+terminalServicePointForUpdate.ID;
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("修改终端维修点出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"修改终端维修点信息成功！");
		return reponse;
	}
	
	private FlexReponse deleteTerminalServicePoint(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_TERMINAL_SERVICE_POINT terminalServicePointForDel = (T_TERMINAL_SERVICE_POINT) request.getBean("terminalServicePointForDel");
		int count = builder.getCount("T_Terminal_Service_Point", "  ID="+terminalServicePointForDel.ID+" and  Org_ID='"+terminalServicePointForDel.ORG_ID+"' and Prov_Name='"+terminalServicePointForDel.PROV_NAME+"' and Area_Name='"+terminalServicePointForDel.AREA_NAME+"' and Tele_No='"+terminalServicePointForDel.TELE_NO+"'");
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("删除终端维修点，获取终端维修点信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count==0){
				throw new Exception("删除终端维修点，该终端维修点已经不存在或已经被修改，请确定后在删除！");
			}
		}
		
		String sql = "delete from T_Terminal_Service_Point where ID = "+terminalServicePointForDel.ID;
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("删除终端维修点失败，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"删除终端维修点信息成功！");
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
//			System.out.println("TempFileForderPath:"+ChobitsConfig.TempFileForderPath);
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
