package cn.ucans.adapter;

import java.util.List;
import java.util.Map;

import org.jdom.Element;

import cn.ucans.flex.services.HappyConfigFlexService;

import chobits.db.DBBuilder;
import chobits.log.Logger;
import chobits.xml.XMLBuilder;

public class AreaAdapter {
	
	private Logger logger = new Logger(AreaAdapter.class.getName());
	
	public AreaAdapter(){}
	
	public String getAreaXMLString(String area_id,int top_id){
		String xml_str = null;
		try{
			DBBuilder builder = new DBBuilder();
			XMLBuilder xb = new XMLBuilder("<node></node>",XMLBuilder.INIT_WITHOUT_HEAD);
			Element root_element = xb.getRootElement();
			String sql = "select Area_Name,Area_Type,Top_Level from T_AREA_CODE_PARM where Area_Code='"+area_id+"'";
			Map<String,Object> map_root = builder.queryMap(sql);
			if(builder.SQL_Error_Code != 0){
				throw new Exception("初始化组织树列表失败，原因："+builder.SQL_Error_Code+","+builder.SQL_Error_Message);
			}else{
				String name = (String) map_root.get("AREA_NAME");
				String type = (String) map_root.get("AREA_TYPE");
				
				root_element.setAttribute("data", area_id);
				root_element.setAttribute("label", name);
				root_element.setAttribute("top", top_id+"");
				root_element.setAttribute("isSelect", "0");
				root_element.setAttribute("type", type);
				
				root_element = this.adapterAreaTree(root_element, area_id, top_id+1);
			}				
			xml_str =  xb.toXML(root_element);
		}catch(Exception e){
			logger.error(e.getMessage());
		}
		return xml_str;
	}
	
	private Element adapterAreaTree(Element element, String parent_id,int top_id)throws Exception{		
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList("select Area_Code,Area_Name,AREA_Type,Top_Level from T_AREA_CODE_PARM where Parent_ID='"+parent_id+"' and Top_Level="+top_id+"");
		if(builder.SQL_Error_Code != 0){
			throw new Exception("初始化地区树列表失败，原因："+builder.SQL_Error_Code+","+builder.SQL_Error_Message);
		}else{
			if(list.size()>0){
				for(int i=0;i<list.size();i++){
					Map<String,Object> map_item = list.get(i);
					String item_data = (String) map_item.get("AREA_CODE");
					String item_name = (String) map_item.get("AREA_NAME");
					String item_level = (String) map_item.get("TOP_LEVEL");
					String item_type = (String) map_item.get("AREA_TYPE");
					
					Element element_child = new Element("node");
					element_child.setAttribute("data", item_data);
					element_child.setAttribute("label", item_name);
					element_child.setAttribute("top", item_level);
					element_child.setAttribute("isSelect", "0");
					element_child.setAttribute("type", item_type);
					element_child = this.adapterAreaTree(element_child,item_data, Integer.parseInt(item_level)+1);
					element.addContent(element_child);
				}
			}
		}
		return element;
	}
}
