package cn.ucans.flex.services;

import java.io.File;
import java.util.List;
import java.util.Map;

import jxl.Workbook;


import chobits.db.DBBuilder;
import chobits.init.ChobitsConfig;
import chobits.log.Logger;
import cn.ucans.bean.T_TERMINAL_PARM;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;

public class TerminalParmService extends FlexService{
	
	private Logger logger = new Logger(TerminalTypeService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------终端参数维护管理任务 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 9020:
					reponse = this.initTerminalParmService(request);
					break;
				case 9021:
					reponse = this.addTerminalParmService(request);
					break;
				case 9022:
					break;
				case 9023:
					reponse = this.updateTerminalParmService(request);
					break;
				case 9024:
					reponse = this.deleteTerminalServicePoint(request);
					break;
				case 9025:
					break;
				case 9026:
					break;
				default:
					break;
			}
			logger.debug("-----------------------------终端参数维护管理任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------终端参数维护管理任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initTerminalParmService(FlexRequest request) throws Exception{
		
		String select_TerminalParmService_Info="Select id,Imei_Number,available,Parameters,create_time,modify_time From t_terminal_parm";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_TerminalParmService_Info);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("查询终端参数维护信息，获取终端参数维护信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse addTerminalParmService(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_TERMINAL_PARM terminalParm = (T_TERMINAL_PARM) request.getBean("terminalParm");
		
		if("*".equals(terminalParm.IMEI_NUMBER)&& "1".equals(terminalParm.AVAILABLE)){
			int count = builder.getCount("T_TERMINAL_PARM", "Imei_Number ='*'and available=1");
			if(builder.SQL_Error_Code != 0){			
				throw new Exception("添加终端参数，获取终端参数信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}else{
				if(count>0){
					throw new Exception("已经存在带\"*\"的终端参数,添加失败！");
				}
			}
		}
		
		String parmeterStr = terminalParm.PARAMETERS.replaceAll("\r", ",");    
		parmeterStr = parmeterStr.replaceAll ( "\n",  ",");  
		
		int available=-1;
		try{
			available=Integer.valueOf(terminalParm.AVAILABLE);
		}catch(Exception e){
			throw new Exception("终端参数维护，添加终端参数失败！！");
		}
		
		String sql = "Insert Into t_terminal_parm(id,Imei_Number,available,Parameters,create_time) Values(Five_Bit_SEQ.NEXTVAL,'"
				+terminalParm.IMEI_NUMBER.trim()+ "',"+available+",'" + parmeterStr + "',sysdate)";
		
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("终端参数维护，添加终端参数出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"添加终端参数信息成功！");
		return reponse;
	}
	
	private FlexReponse updateTerminalParmService(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_TERMINAL_PARM terminalParm = (T_TERMINAL_PARM) request.getBean("terminalParmForUpdate");
		T_TERMINAL_PARM oldTerminalParmForUpdate = (T_TERMINAL_PARM) request.getBean("oldTerminalParmForUpdate");
		
		int count = builder.getCount("T_TERMINAL_PARM", " id="+oldTerminalParmForUpdate.ID + " and Imei_Number='"+oldTerminalParmForUpdate.IMEI_NUMBER+"' and Parameters='"+oldTerminalParmForUpdate.PARAMETERS+"' and Available='"+oldTerminalParmForUpdate.AVAILABLE+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("终端参数维护，修改终端参数，获取终端参数信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count==0){
				throw new Exception("修改终端参数，该终端参数已经不存在或已经被修改，请确认后再修改！");
			}
		}
		
		if(("*").equals(terminalParm.IMEI_NUMBER)&& "1".equals(terminalParm.AVAILABLE)){
			
			int count3 = builder.getCount("T_TERMINAL_PARM", "Imei_Number ='*' and id="+ terminalParm.ID);
			if(count3 <= 0){
				int count2 = builder.getCount("T_TERMINAL_PARM", "Imei_Number ='*' and available=1");
				if(builder.SQL_Error_Code != 0){			
					throw new Exception("修改终端参数，获取终端参数信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
				}else{
					if(count2>0){
						throw new Exception("已经存在带\"*\"的终端参数,修改失败！");
					}
				}
			}
		}
		
		String parmeterStr = terminalParm.PARAMETERS.replaceAll("\r", ",");    
		parmeterStr = parmeterStr.replaceAll ( "\n",  ",");
		
		String update_sql = "update t_terminal_parm set ";
		update_sql += " Imei_Number ='"+terminalParm.IMEI_NUMBER.trim()+"',";
		update_sql += " available ="+terminalParm.AVAILABLE+",";
		update_sql += " Parameters ='"+parmeterStr+"',";
		update_sql += " modify_time =sysdate";
		update_sql += " where ID ="+ terminalParm.ID;
		builder.excute(update_sql);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("配置参数，修改终端参数信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		
		FlexReponse reponse = new FlexReponse(request,0,"修改终端参数信息成功！");
		
		return reponse;
	}
	
	private FlexReponse deleteTerminalServicePoint(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_TERMINAL_PARM terminalParm = (T_TERMINAL_PARM) request.getBean("terminalParmForDel");
		
		int count = builder.getCount("T_TERMINAL_PARM", "id="+terminalParm.ID + " and Imei_Number='"+terminalParm.IMEI_NUMBER+"' and Parameters='"+terminalParm.PARAMETERS+"' and Available='"+terminalParm.AVAILABLE+"'");
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("终端参数维护，删除终端参数，获取终端参数维护信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count<=0){
				throw new Exception("终端参数维护，删除终端参数维护信息，该终端参数已经不存在或已经被修改，请确认后再删除！");
			}
		}
		
		
		String sql = "delete from T_TERMINAL_PARM where ID = "+terminalParm.ID;
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("终端参数维护，删除终端参数维护信息失败，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"删除终端参数维护信息成功！");
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
