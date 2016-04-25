package cn.ucans.adapter;

import java.util.List;
import java.util.Map;

import org.jdom.Element;

import chobits.db.DBBuilder;
import chobits.log.Logger;
import chobits.xml.XMLBuilder;

public class OrgAdapter {
	
	public int Error_Code = 0;
	public String Error_Message = "";
	
	private DBBuilder builder = null;
	private XMLBuilder xmlbuilder = null;
	
	private String xml = "<?xml version='1.0' encoding='UTF-8'?><node></node>";
	private Element element = null;
	
	private Logger logger = new Logger(OrgAdapter.class.getName());
	
	public OrgAdapter(){
		xmlbuilder = new XMLBuilder(xml,XMLBuilder.INIT_WITH_HEAD);
		builder = new DBBuilder();		
	}
	
	public String getTreeXMLString(String org_id,int top_id,String solution){
		String xml_str = null;
		
		try{
			this.element = xmlbuilder.getRootElement();
			String sql = "select Org_Name,Org_Type from T_Org_Info where Org_ID='"+org_id+"' and Top_Level="+top_id+" "+solution;
			Map<String,Object> map_root = builder.queryMap(sql);
			if(builder.SQL_Error_Code != 0){
				throw new Exception("初始化组织树列表失败，原因："+builder.SQL_Error_Code+","+builder.SQL_Error_Message);
			}else{
				if(map_root != null){
					String name = (String) map_root.get("ORG_NAME");
					String type = (String) map_root.get("ORG_TYPE");
					
					this.element.setAttribute("id", "XXXXX");
					this.element.setAttribute("data", org_id);
					this.element.setAttribute("label", name);
					this.element.setAttribute("top", top_id+"");
					this.element.setAttribute("isSelect", "0");
					this.element.setAttribute("type", type);
					
					this.adapterOrgTree(this.element, org_id, top_id+1,solution);
				}
			}	
			xml_str =  xmlbuilder.toXML(this.element);
		}catch(Exception e){
			logger.error(e.getMessage());
		}
		return xml_str;
	}
	
	private void adapterOrgTree(Element element, String parent_id,int top_id,String solution)throws Exception{		
		List<Map<String,Object>> list = builder.queryList("select Org_ID,Org_Name,Top_Level,Org_Type from T_Org_Info where Superior_Org_ID='"+parent_id+"' and Top_Level="+top_id+"  "+solution+" order by Create_Time");
		if(builder.SQL_Error_Code != 0){
			throw new Exception("初始化组织树列表失败，原因："+builder.SQL_Error_Code+","+builder.SQL_Error_Message);
		}else{
			if(list.size()>0){
				for(int i=0;i<list.size();i++){
					Map<String,Object> map_item = list.get(i);
					
					String item_data = (String) map_item.get("ORG_ID");
					String item_name = (String) map_item.get("ORG_NAME");
					String item_level = (String) map_item.get("TOP_LEVEL");
					String item_type = (String) map_item.get("ORG_TYPE");
					
					Element element_child = new Element("node");
					element_child.setAttribute("id", "XXXXX");
					element_child.setAttribute("data", item_data);
					element_child.setAttribute("label", item_name);
					element_child.setAttribute("top", item_level);
					element_child.setAttribute("isSelect", "0");
					element_child.setAttribute("type", item_type);
					element.addContent(element_child);
					
					this.adapterOrgTree(element_child,item_data, Integer.parseInt(item_level)+1,solution);
				}
			}
		}		
	}
}
