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
import cn.ucans.bean.T_ORG_REWARD;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class OrgRewardService extends FlexService{
	
	private Logger logger = new Logger(TerminalTypeService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------业务分成比例管理任务 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 9060:
					reponse = this.initOrgRewardInfo(request);
					break;
				case 9061:
					reponse = this.queryOrgRewardInfo(request);
					break;
				case 9062:
					reponse = this.updateOrgRewardPersent(request);
					break;
				case 9063:
					
					break;
				case 9064:
					break;
				case 9065:
					break;
				case 9066:
					break;
				default:
					break;
			}
			logger.debug("-----------------------------业务分成比例管理任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------业务分成比例管理任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initOrgRewardInfo(FlexRequest request) throws Exception{
		
		String select_OrgReward_Info_SQL="Select a.Org_ID,a.Org_Name,a.Org_Type,a.Status,b.Reward_Type,b.Reward_Percent From T_Org_Info a,T_Org_Reward b where a.Org_ID=b.Org_ID";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_OrgReward_Info_SQL);
		
		String []strSQLParm=new String[4];
		
		strSQLParm[0]="2";
		strSQLParm[1]="3";
		strSQLParm[2]="4";
		
		List<Map<String,Object>> orgNameList = new ParamHandler().getT_OrgID__OrgNames(strSQLParm);
		
		List<Map<String,Object>> queryOrgNameList = new LinkedList<Map<String,Object>>();
		Map<String,Object> queryAllOrgListMap=new HashMap<String,Object>();
		queryAllOrgListMap.put("label", "全部");
		queryAllOrgListMap.put("data", "");
		queryOrgNameList.add(queryAllOrgListMap);
		
		queryOrgNameList.addAll(orgNameList);
		
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("查询业务分成比例信息，获取业务分成比例信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		reponse.set("queryOrgNameList", queryOrgNameList);
		return reponse;
	}
	
	private FlexReponse queryOrgRewardInfo(FlexRequest request) throws Exception{
		
		DBBuilder builder = new DBBuilder();
		String orgType = (String)request.getBean("orgType");
		String orgName = (String)request.getBean("orgName");
		String hotelGrade = (String)request.getBean("hotelGrade");
		String orgStatus = (String)request.getBean("orgStatus");
		String rewardType = (String)request.getBean("rewardType");
		String linkmanName = (String)request.getBean("linkmanName");
		
		String querySQL="";
		if(!"".equals(linkmanName)&&linkmanName.length()>0){
			querySQL="Select a.Org_ID,a.Org_Name,a.Org_Type,a.Status,b.Reward_Type,b.Reward_Percent From T_Org_Info a,T_Org_Reward b " +
					"where a.Org_ID=b.Org_ID and a.Org_Id in (select Org_Id from T_Org_Linkman where Linkman_Name like '%"+linkmanName+"%')";
		}else{
			querySQL="Select a.Org_ID,a.Org_Name,a.Org_Type,a.Status,b.Reward_Type,b.Reward_Percent From T_Org_Info a,T_Org_Reward b where a.Org_ID=b.Org_ID";
		}
		
		
		if(orgType != null && !"".equals(orgType)){
			querySQL +=" and a.Org_Type='"+orgType+"'";
		}
		if(orgName != null && !"".equals(orgName)){
			querySQL +=" and a.Org_ID='"+orgName+"'";		
		}
		if(hotelGrade != null && !"".equals(hotelGrade)){
			int hotelN=0;
			try{
				 hotelN= Integer.valueOf(hotelGrade);
			}catch(Exception e){
				throw new Exception("查询业务分成比例时类型转换出错，原因："+e.getMessage());
			}
			querySQL +=" and a.Hotel_Grade="+hotelN;
		}
		if(orgStatus != null && !"".equals(orgStatus)){
			querySQL +=" and a.Status='"+orgStatus+"'";
		}
		if(rewardType != null && !"".equals(rewardType)){
			int rewardN;
			try{
				rewardN = Integer.valueOf(rewardType);
			}catch(Exception e){
				throw new Exception("查询业务分成比例时类型转换出错，原因："+ e.getMessage());
			}
			querySQL +=" and b.Reward_Type="+rewardN;
		}
		
		List<Map<String,Object>> list = builder.queryList(querySQL);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("查询业务分成比例出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		return reponse;
	}

	
	private FlexReponse updateOrgRewardPersent(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_ORG_REWARD orgRewardForUpdate = (T_ORG_REWARD) request.getBean("orgRewardForUpdate");
		
		int count = builder.getCount("T_ORG_REWARD", " Org_ID='"+orgRewardForUpdate.ORG_ID+"' and Reward_Type="+ orgRewardForUpdate.REWARD_TYPE);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("修改业务分成比例，获取业务分成比例信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count==0){
				throw new Exception("修改业务分成比例，该业务分成比例记录已经不存在！");
			}
		}
		
		String update_sql = "update T_ORG_REWARD set ";
		update_sql += " Reward_Percent ="+orgRewardForUpdate.REWARD_PERCENT ;
		update_sql += " where Org_ID='"+orgRewardForUpdate.ORG_ID+"' and Reward_Type="+ orgRewardForUpdate.REWARD_TYPE;
		builder.excute(update_sql);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("修改业务分成比例信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		
		FlexReponse reponse = new FlexReponse(request,0,"修改业务分成比例信息成功！");
		
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
