package cn.ucans.flex.services;

import java.util.List;
import java.util.Map;


import chobits.db.DBBuilder;
import chobits.log.Logger;
import cn.ucans.adapter.AreaAdapter;
import cn.ucans.bean.T_AREA_CODE_PARM;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;

public class HappyConfigFlexService extends FlexService{
	
	private Logger logger = new Logger(HappyConfigFlexService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------参数配置任务 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 7000:
					reponse = this.initArea(request);
					break;
				case 7001:
					reponse = this.queryArea(request);
					break;
				case 7002:
					reponse = this.addArea(request);
					break;
				case 7003:
					reponse = this.updateArea(request);
					break;
				case 7004:
					reponse = this.deleteArea(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------参数配置任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------参数配置任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initArea(FlexRequest request) throws Exception{
		T_AREA_CODE_PARM area = (T_AREA_CODE_PARM) request.getBean("area");
		AreaAdapter aa = new AreaAdapter();
		String xml = aa.getAreaXMLString("0086", 0);
		if(xml == null){
			throw new Exception("初始化地区树失败，系统备有录入任何信息！");
		}else{
			logger.debug("生成地区树"+xml);
		}
		
		String select_sql = "select Area_Code,Area_Name,Area_Type,Top_Level,Parent_ID from T_AREA_CODE_PARM where Parent_ID='0086' and Top_Level=1";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("配置参数，查询地区代码，获取地区市信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("xml", xml);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse queryArea(FlexRequest request) throws Exception{
		T_AREA_CODE_PARM area = (T_AREA_CODE_PARM) request.getBean("area");
		AreaAdapter aa = new AreaAdapter();
		String xml = aa.getAreaXMLString(area.AREA_CODE, Integer.parseInt(area.TOP_LEVEL));
		if(xml == null){
			throw new Exception("初始化地区树失败，系统备有录入任何信息！");
		}else{
			logger.debug("生成地区树"+xml);
		}
		
		String select_sql = "select Area_Code,Area_Name,Area_Type,Top_Level,Parent_ID from T_AREA_CODE_PARM where Parent_ID='"+area.AREA_CODE+"' and Top_Level="+(Integer.parseInt(area.TOP_LEVEL)+1);
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("配置参数，查询地区代码，获取地区市信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("xml", xml);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse addArea(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_AREA_CODE_PARM area = (T_AREA_CODE_PARM) request.getBean("area");
		
		int count = builder.getCount("T_AREA_CODE_PARM", "Area_Code='"+area.AREA_CODE+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("配置参数，添加地区代码，获取地区市信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count>0){
				throw new Exception("配置参数，添加地区校验地区代码，该编号（"+area.AREA_CODE+"）已经存在！");
			}
		}
		
		String sql = "insert into T_AREA_CODE_PARM (Area_Code,Area_Name,Area_Type,Top_Level,Parent_ID) values ";
		sql += "('"+area.AREA_CODE+"','"+area.AREA_NAME+"',"+area.AREA_TYPE+","+area.TOP_LEVEL+",'"+area.PARENT_ID+"')";
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("配置参数，添加地区代码出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		AreaAdapter aa = new AreaAdapter();
		String xml_add = aa.getAreaXMLString(area.PARENT_ID, Integer.parseInt(area.TOP_LEVEL)-1);
		if(xml_add == null){
			throw new Exception("初始化地区树失败，系统备有录入任何信息！");
		}else{
			logger.debug("生成地区树"+xml_add);
		}
		FlexReponse reponse = new FlexReponse(request,0,"添加地区（"+area.AREA_CODE+"，"+area.AREA_NAME+"）信息成功！");
		reponse.set("xml", xml_add);
		return reponse;
	}
	
	private FlexReponse updateArea(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_AREA_CODE_PARM area = (T_AREA_CODE_PARM) request.getBean("area");
		
		int count = builder.getCount("T_AREA_CODE_PARM", "Area_Code='"+area.AREA_CODE+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("配置参数，更新地区代码，获取地区市信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count==0){
				throw new Exception("配置参数，更新地区校验地区代码，该编号（"+area.AREA_CODE+"）已经不存在！");
			}
		}
		
		String select_sql = "select Area_Code,Area_Name,Area_Type,Top_Level,Parent_ID from T_AREA_CODE_PARM where Area_Code='"+area.AREA_CODE+"'";
		Map<String,Object> map = builder.queryMap(select_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("配置参数，更新地区，查询地区"+area.AREA_CODE+"信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		String code = (String) map.get("AREA_CODE");
		String name = (String) map.get("AREA_NAME");
		String type = (String) map.get("AREA_TYPE");
		String top = (String) map.get("TOP_LEVEL");
		String parent_id = (String) map.get("PARENT_ID");
		
		if(type.equals("1") && area.AREA_TYPE.equals("2")){
			int count_child = builder.getCount("T_AREA_CODE_PARM", "Parent_ID='"+area.AREA_CODE+"' and Top_Level="+(Integer.parseInt(area.TOP_LEVEL)+1));
			if(builder.SQL_Error_Code != 0){			
				throw new Exception("配置参数，更新地区代码，获取地区市下级地区信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}else{
				if(count>0){
					throw new Exception("配置参数，该地区（"+area.AREA_CODE+"）已经存在"+count_child+"个下级地区,不能由省转换为直辖市！s");
				}
			}
		}
		String update_sql = "update T_AREA_CODE_PARM set ";
		update_sql += " Area_Name = '"+area.AREA_NAME+"',";
		update_sql += " Area_Type = "+area.AREA_TYPE+" ";
		update_sql += " where Area_Code='"+area.AREA_CODE+"'";
		builder.excute(update_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("配置参数，更新地区代码出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		AreaAdapter aa = new AreaAdapter();
		String xml_update = aa.getAreaXMLString(parent_id, Integer.parseInt(area.TOP_LEVEL)-1);
		if(xml_update == null){
			throw new Exception("初始化地区树失败，系统备有录入任何信息！");
		}else{
			logger.debug("生成地区树"+xml_update);
		}
		FlexReponse reponse = new FlexReponse(request,0,"更新地区（"+area.AREA_CODE+"，"+area.AREA_NAME+"）信息成功！");
		reponse.set("xml", xml_update);
		return reponse;
	}
	
	private FlexReponse deleteArea(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_AREA_CODE_PARM area = (T_AREA_CODE_PARM) request.getBean("area");
		
		int count = builder.getCount("T_AREA_CODE_PARM", "Area_Code='"+area.AREA_CODE+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("配置参数，删除地区代码，获取地区市信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count==0){
				throw new Exception("配置参数，删除地区校验地区代码，该编号（"+area.AREA_CODE+"）已经不存在！");
			}
		}
		
		int count_child = builder.getCount("T_AREA_CODE_PARM", "Parent_ID='"+area.AREA_CODE+"' and Top_Level="+(Integer.parseInt(area.TOP_LEVEL)+1));
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("配置参数，添加地区代码，获取地区市信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count_child>0){
				throw new Exception("配置参数，该地区（"+area.AREA_CODE+"）已经存在"+count_child+"个下级地区，请先处理这些地区信息，再进行本操作！");
			}
		}
		
		String select_sql = "select Area_Code,Area_Name,Area_Type,Top_Level,Parent_ID from T_AREA_CODE_PARM where Area_Code='"+area.AREA_CODE+"'";
		Map<String,Object> map = builder.queryMap(select_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("配置参数，删除地区，查询地区"+area.AREA_CODE+"信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		String code = (String) map.get("AREA_CODE");
		String name = (String) map.get("AREA_NAME");
		String type = (String) map.get("AREA_TYPE");
		String top = (String) map.get("TOP_LEVEL");
		String parent_id = (String) map.get("PARENT_ID");
		
		String sql = "delete from T_AREA_CODE_PARM where Area_Code = '"+area.AREA_CODE+"' ";
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("配置参数，删除地区代码出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		AreaAdapter aa = new AreaAdapter();
		String xml_delete = aa.getAreaXMLString(parent_id, Integer.parseInt(area.TOP_LEVEL)-1);
		if(xml_delete == null){
			throw new Exception("初始化地区树失败，系统备有录入任何信息！");
		}else{
			logger.debug("生成地区树"+xml_delete);
		}
		FlexReponse reponse = new FlexReponse(request,0,"删除地区（"+area.AREA_CODE+"，"+area.AREA_NAME+"）信息成功！");
		reponse.set("xml", xml_delete);
		return reponse;
	}
}
