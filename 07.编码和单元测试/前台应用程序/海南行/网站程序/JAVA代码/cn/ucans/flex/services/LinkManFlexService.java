package cn.ucans.flex.services;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import chobits.db.DBBuilder;
import chobits.log.Logger;
import cn.ucans.bean.T_ORG_INFO;
import cn.ucans.bean.T_ORG_LINKMAN;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;

public class LinkManFlexService extends FlexService{
	
	private Logger logger = new Logger(LinkManFlexService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------人员信息管理任务 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{			
			super.checkRole(request);
			int ope_id = request.ope_id;			
			switch(ope_id){
				case 1006:
					reponse = this.addHuman(request);
					break;
				case 1007:
					reponse = this.updateHuman(request);
					break;
				case 1008:
					reponse = this.deleteHuman(request);
					break;
				case 1009:
					reponse = this.queryHuman(request);
					break;
				case 1010:
					reponse = this.leadData(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------人员信息管理任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------人员信息管理任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse addHuman(FlexRequest request) throws Exception{		
		DBBuilder builder = new DBBuilder();
		T_ORG_LINKMAN human = (T_ORG_LINKMAN) request.getBean("human");
		
		String sql_insert_human = null;
		sql_insert_human = "insert into T_ORG_LINKMAN (ID,ORG_ID,LINKMAN_NAME,DEPARTMENT,TITLE,POSITION,SERVICE_NOTES,TELE_NO,MOBILE_NO,FAX_NO,EMAIL,CREATE_TIME,LAST_UPDATE_TIME) values ";
		sql_insert_human += "(Five_Bit_SEQ.nextVal,'"+human.ORG_ID+"','"+human.LINKMAN_NAME+"','"+human.DEPARTMENT+"','"+human.TITLE+"','"+human.POSITION+"','"+human.SERVICE_NOTES+"','"+human.TELE_NO+"','"+human.MOBILE_NO+"','"+human.FAX_NO+"','"+human.EMAIL+"',sysdate,sysdate)";
		builder.excute(sql_insert_human);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("添加人员信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"添加人员("+human.ID+"，"+human.LINKMAN_NAME+")成功！");
		return reponse;
	}
	
	private FlexReponse updateHuman(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_ORG_LINKMAN human = (T_ORG_LINKMAN) request.getBean("human");
		
		String sql_update_human = "update T_ORG_LINKMAN set ";
		
		sql_update_human += " Linkman_Name = '"+human.LINKMAN_NAME+"',";//
		sql_update_human += " Department = '"+human.DEPARTMENT+"',";//
		sql_update_human += " Title = '"+human.TITLE+"',";//
		sql_update_human += " Position = '"+human.POSITION+"',";//
		sql_update_human += " Service_Notes = '"+human.SERVICE_NOTES+"',";//
		sql_update_human += " Tele_No = '"+human.TELE_NO+"',";//
		sql_update_human += " Mobile_No = '"+human.MOBILE_NO+"',";//
		sql_update_human += " Fax_No = '"+human.FAX_NO+"',";//
		sql_update_human += " Email = '"+human.EMAIL+"',";//
		sql_update_human += " Last_Update_Time = sysdate ";//
		
		sql_update_human += " where ID = '"+human.ID+"'";
		builder.excute(sql_update_human);			
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("更新人员信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		String sql = "select ID,ORG_ID,LINKMAN_NAME,DEPARTMENT,TITLE,POSITION,SERVICE_NOTES,TELE_NO,MOBILE_NO,FAX_NO,EMAIL,CREATE_TIME,LAST_UPDATE_TIME from T_ORG_LINKMAN where ID="+human.ID;
		Map<String,Object> map = builder.queryMap(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("更新人员信息，查询更改后的信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"更新人员("+human.ID+"，"+human.LINKMAN_NAME+")信息成功！");
		reponse.set("map", map);
		return reponse;
	}

	private FlexReponse deleteHuman(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_ORG_LINKMAN human = (T_ORG_LINKMAN) request.getBean("human");
		
		String sql_delete_human = "delete from T_ORG_LINKMAN where ID = '"+human.ID+"'";
		builder.excute(sql_delete_human);		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("删除人员信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"删除人员("+human.ID+"，"+human.LINKMAN_NAME+")信息成功！");
		return reponse;
	}
	
	private FlexReponse queryHuman(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();	
		T_ORG_LINKMAN human = (T_ORG_LINKMAN) request.getBean("human");
		T_ORG_INFO org = (T_ORG_INFO) request.getBean("org");
		
		String sql = "select T_ORG_LINKMAN.ID as ID,T_ORG_LINKMAN.Org_ID as Org_ID,T_ORG_LINKMAN.Linkman_Name as Linkman_Name,T_ORG_LINKMAN.Department as Department,";
		sql += "T_ORG_LINKMAN.TITLE as Title,T_ORG_LINKMAN.POSITION as POSITION,T_ORG_LINKMAN.SERVICE_NOTES as SERVICE_NOTES,T_ORG_LINKMAN.TELE_NO as TELE_NO,";
		sql += "T_ORG_LINKMAN.MOBILE_NO as MOBILE_NO,T_ORG_LINKMAN.FAX_NO as FAX_NO,T_ORG_LINKMAN.EMAIL as EMAIL,T_ORG_LINKMAN.CREATE_TIME as CREATE_TIME,";
		sql += "T_ORG_LINKMAN.LAST_UPDATE_TIME as LAST_UPDATE_TIME,T_ORG_INFO.ORG_NAME as ORG_NAME,T_ORG_INFO.STATUS as Status ";
		sql += " from T_ORG_LINKMAN,T_ORG_INFO ";
		sql += " where T_ORG_LINKMAN.Org_ID=T_ORG_INFO.Org_ID ";
		boolean haswhere = false;
		haswhere = true;
		if(human.LINKMAN_NAME != null && human.LINKMAN_NAME.trim().length()>0){
			if(haswhere){
				sql += " and T_ORG_LINKMAN.LINKMAN_NAME='"+human.LINKMAN_NAME+"' ";
			}else{
				sql += " where T_ORG_LINKMAN.LINKMAN_NAME='"+human.LINKMAN_NAME+"' ";
				haswhere = true;
			}			
		}
		if(org.STATUS != null && org.STATUS.trim().length()>0){
			if(haswhere){
				sql += " and T_ORG_INFO.STATUS='"+org.STATUS+"' ";
			}else{
				sql += " where T_ORG_INFO.STATUS='"+org.STATUS+"' ";
				haswhere = true;
			}			
		}
		if(human.ORG_ID != null && human.ORG_ID.trim().length()>0){
			if(haswhere){
				sql += " and T_ORG_LINKMAN.ORG_ID='"+human.ORG_ID+"' ";
			}else{
				sql += " where T_ORG_LINKMAN.ORG_ID='"+human.ORG_ID+"'";
				haswhere = true;
			}			
		}
		sql += "order by T_ORG_LINKMAN.Create_Time";
		
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("查询人员信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse leadData(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();	
		String xml = (String) request.getBean("xml");
		
		String linkurl = builder.leadDataToFile(xml, "LinkManData");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("导出数据发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("url", linkurl);
		return reponse;
	}
}
