package cn.ucans.adapter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.jdom.Element;

import chobits.db.DBBuilder;
import chobits.log.Logger;
import chobits.xml.XMLBuilder;

public class RoleAdapter {
	
	public int Error_Code = 0;
	public String Error_Message = "";
	
	private DBBuilder builder = null;
	private XMLBuilder xmlbuilder = null;
	
	private String xml = "<?xml version='1.0' encoding='UTF-8'?><node></node>";
	private Element element = null;
	private List<String> list_sql = new ArrayList<String>();
	
	private Logger logger = new Logger(RoleAdapter.class.getName());
	
	public RoleAdapter(){
		xmlbuilder = new XMLBuilder(xml,XMLBuilder.INIT_WITH_HEAD);
		builder = new DBBuilder();		
	}
	
	public RoleAdapter(String element_xml_format){
		xmlbuilder = new XMLBuilder(element_xml_format,XMLBuilder.INIT_WITHOUT_HEAD);
		builder = new DBBuilder();		
	}
	
	public List<String> getSetupSQLList(String role_id){
		this.list_sql.clear();
		this.element = xmlbuilder.getRootElement();
		if(this.element != null){
			String isSelect_root = this.element.getAttributeValue("isSelect");
			if(isSelect_root.equals("0")){
				return null;
			}else{
				String menu_id = this.element.getAttributeValue("id");
				String Program_Name = this.element.getAttributeValue("data");
				String menu_name = this.element.getAttributeValue("label");
				int menu_top = Integer.parseInt(this.element.getAttributeValue("top"));
				String sql = "insert into T_Role_Menu (Role_ID,Menu_ID,Menu_Name,Top_Level,Program_Name,Parent_ID) values ";
				sql += "('"+role_id+"',"+menu_id+",'"+menu_name+"',"+menu_top+",'"+Program_Name+"',-1)";
				this.list_sql.add(sql);
				this.element = this.getSelectedSQL(this.element,role_id);
			}
		}
		return this.list_sql;
	}
	
	public List<String> getMenuChangeSQLList(String role_id){
		this.list_sql.clear();
		this.element = xmlbuilder.getRootElement();
		if(this.element != null){
			String isSelect_root = this.element.getAttributeValue("isSelect");
			if(isSelect_root.equals("0")){
				return null;
			}else{
				String menu_id = this.element.getAttributeValue("id");
				String Program_Name = this.element.getAttributeValue("data");
				String menu_name = this.element.getAttributeValue("label");
				int menu_top = Integer.parseInt(this.element.getAttributeValue("top"));
				String sql = "update T_Role_Menu set Menu_Name = '"+menu_name+"' where Menu_ID='"+menu_id+"' and Role_ID='"+role_id+"'";
				this.list_sql.add(sql);
				this.element = this.getMenuChangeSQL(this.element,role_id);
			}
		}
		return this.list_sql;
	}
	
	private Element getSelectedSQL(Element element, String role_id){
		String parent_id = element.getAttributeValue("id");
		List<Element> list_child = element.getChildren();
		for(int i=0;i<list_child.size();i++){
			Element element_item = list_child.get(i);
			String isSelect_item = element_item.getAttributeValue("isSelect");
			if(!isSelect_item.equals("0")){
				String menu_id = element_item.getAttributeValue("id");
				String Program_Name = element_item.getAttributeValue("data");
				String menu_name = element_item.getAttributeValue("label");
				int menu_top = Integer.parseInt(element_item.getAttributeValue("top"));
				String sql = "insert into T_Role_Menu (Role_ID,Menu_ID,Menu_Name,Top_Level,Program_Name,Parent_ID) values ";
				sql += "('"+role_id+"',"+menu_id+",'"+menu_name+"',"+menu_top+",'"+Program_Name+"',"+parent_id+")";
				this.list_sql.add(sql);
				element = this.getSelectedSQL(element_item,role_id);
			}
		}
		return element;		
	}
	
	private Element getMenuChangeSQL(Element element, String role_id){
		String parent_id = element.getAttributeValue("id");
		List<Element> list_child = element.getChildren();
		for(int i=0;i<list_child.size();i++){
			Element element_item = list_child.get(i);
			String isSelect_item = element_item.getAttributeValue("isSelect");
			if(!isSelect_item.equals("0")){
				String menu_id = element_item.getAttributeValue("id");
				String Program_Name = element_item.getAttributeValue("data");
				String menu_name = element_item.getAttributeValue("label");
				int menu_top = Integer.parseInt(element_item.getAttributeValue("top"));
				String sql = "update T_Role_Menu set Menu_Name = '"+menu_name+"' where Menu_ID='"+menu_id+"' and Role_ID='"+role_id+"'";
				this.list_sql.add(sql);
				element = this.getMenuChangeSQL(element_item,role_id);
			}
		}
		return element;		
	}
	
	public String getNewRoleXMLString(String xml_format){
		this.element = xmlbuilder.getRootElement();
		if(this.element != null){
			String isSelect_root = this.element.getAttributeValue("isSelect");
			if(isSelect_root.equals("0")){
				return "<?xml version='1.0' encoding='UTF-8'?><node></node>";
			}else{
				this.element = this.getSelectedFieldXML(this.element);
			}
		}
		String xml = xmlbuilder.toXML(this.element);
		return xml;
	}
	
	private Element getSelectedFieldXML(Element element){
		String isSelect = element.getAttributeValue("isSelect");
		List<Element> list_child = element.getChildren();
		for(int i=0;i<list_child.size();i++){
			Element element_item = list_child.get(i);
			String isSelect_item = element_item.getAttributeValue("isSelect");
			if(isSelect_item.equals("0")){
				element.removeContent(element_item);
			}else{
				element = this.getSelectedFieldXML(element_item);
			}
		}
		return element;		
	}
	
	public String getInitUpdateRoleXMLString(String role_id){
		String xml_str = null;
		
		try{			
			List<String> list_role_id = builder.queryListForSingleField("Menu_ID", "T_Role_Menu", "Role_ID='"+role_id+"' order by Menu_ID");
			if(list_role_id==null || list_role_id.size()==0){
				xml_str = this.getInitRoleXMLString("-1", 0, "00A");
			}else{
				
				Element element = new Element("node");
				element.setAttribute("id", "-1");
				element.setAttribute("data", "Happy");
				element.setAttribute("label", "海南行平台");
				element.setAttribute("top", "0");					
				element.setAttribute("isSelect", "1");
				
				List<Map<String,Object>> list_00A_1 = builder.queryList("select Menu_ID,Menu_Name,Top_Level,Program_Name from T_Role_Menu where Role_ID='00A' and Parent_ID=-1 and Top_Level=1 order by Menu_ID ");
				if(list_00A_1.size()>0){
					for(int k=0;k<list_00A_1.size();k++){
						Map<String,Object> map_1 = list_00A_1.get(k);
						String menu_id = (String) map_1.get("MENU_ID");
						String menu_name = (String) map_1.get("MENU_NAME");
						String menu_top = (String) map_1.get("TOP_LEVEL");
						String program_name = (String) map_1.get("PROGRAM_NAME");
						
						Element element_1 = new Element("node");
						element_1.setAttribute("id", menu_id);
						element_1.setAttribute("data", program_name);
						element_1.setAttribute("label", menu_name);
						element_1.setAttribute("top", menu_top);
						
						if(list_role_id.contains(menu_id)){
							element_1.setAttribute("isSelect", "1");
						}else{
							element_1.setAttribute("isSelect", "0");
						}
						
						List<Map<String,Object>> list_00A_2 = builder.queryList("select Menu_ID,Menu_Name,Top_Level,Program_Name from T_Role_Menu where Role_ID='00A' and Parent_ID="+menu_id+" and Top_Level=2 order by Menu_ID ");
						if(list_00A_2.size()>0){
							for(int m=0;m<list_00A_2.size();m++){
								Map<String,Object> map_2 = list_00A_2.get(m);
								String menu_id_2 = (String) map_2.get("MENU_ID");
								String menu_name_2 = (String) map_2.get("MENU_NAME");
								String menu_top_2 = (String) map_2.get("TOP_LEVEL");
								String program_name_2 = (String) map_2.get("PROGRAM_NAME");
								
								Element element_2 = new Element("node");
								element_2.setAttribute("id", menu_id_2);
								element_2.setAttribute("data", program_name_2);
								element_2.setAttribute("label", menu_name_2);
								element_2.setAttribute("top", menu_top_2);
								
								if(list_role_id.contains(menu_id_2)){
									element_2.setAttribute("isSelect", "1");
								}else{
									element_2.setAttribute("isSelect", "0");
								}
								element_1.addContent(element_2);
							}
						}
						element.addContent(element_1);
					}
				}				
				XMLBuilder xb = new XMLBuilder();
				xml_str = xb.toXML(element);
			}			
		}catch(Exception e){
			logger.error(e.getMessage());
		}
		return xml_str;
	}
	
	public String getInitchangeMenuRoleXMLString(String role_id){
		String xml_str = null;
		
		try{			
			List<Map<String,Object>> list_00A_1 = builder.queryList("select Menu_ID,Menu_Name,Top_Level,Program_Name from T_Role_Menu where Role_ID='"+role_id+"' and Parent_ID=-1 and Top_Level=1 order by Menu_ID ");
			if(list_00A_1==null || list_00A_1.size()==0){
				
			}else{
				
				Element element = new Element("node");
				element.setAttribute("id", "-1");
				element.setAttribute("data", "Happy");
				element.setAttribute("label", "海南行平台");
				element.setAttribute("top", "0");					
				element.setAttribute("isSelect", "1");
				
				
				if(list_00A_1.size()>0){
					for(int k=0;k<list_00A_1.size();k++){
						Map<String,Object> map_1 = list_00A_1.get(k);
						String menu_id = (String) map_1.get("MENU_ID");
						String menu_name = (String) map_1.get("MENU_NAME");
						String menu_top = (String) map_1.get("TOP_LEVEL");
						String program_name = (String) map_1.get("PROGRAM_NAME");
						
						Element element_1 = new Element("node");
						element_1.setAttribute("id", menu_id);
						element_1.setAttribute("data", program_name);
						element_1.setAttribute("label", menu_name);
						element_1.setAttribute("top", menu_top);
						element_1.setAttribute("isSelect", "1");
						
						List<Map<String,Object>> list_00A_2 = builder.queryList("select Menu_ID,Menu_Name,Top_Level,Program_Name from T_Role_Menu where Role_ID='"+role_id+"' and Parent_ID="+menu_id+" and Top_Level=2 order by Menu_ID ");
						if(list_00A_2.size()>0){
							for(int m=0;m<list_00A_2.size();m++){
								Map<String,Object> map_2 = list_00A_2.get(m);
								String menu_id_2 = (String) map_2.get("MENU_ID");
								String menu_name_2 = (String) map_2.get("MENU_NAME");
								String menu_top_2 = (String) map_2.get("TOP_LEVEL");
								String program_name_2 = (String) map_2.get("PROGRAM_NAME");
								
								Element element_2 = new Element("node");
								element_2.setAttribute("id", menu_id_2);
								element_2.setAttribute("data", program_name_2);
								element_2.setAttribute("label", menu_name_2);
								element_2.setAttribute("top", menu_top_2);
								element_2.setAttribute("isSelect", "1");
								element_1.addContent(element_2);
							}
						}
						element.addContent(element_1);
					}
				}				
				XMLBuilder xb = new XMLBuilder();
				xml_str = xb.toXML(element);
			}			
		}catch(Exception e){
			logger.error(e.getMessage());
		}
		return xml_str;
	}
	
	private void adapterSelectedRoleTree(Element element, String parent_id,int top_id, List<String> list_equal)throws Exception{		
		List<Map<String,Object>> list = builder.queryList("select Menu_ID,Menu_Name,Top_Level,Program_Name from T_Role_Menu where Parent_ID="+parent_id+" and Top_Level="+top_id+" and Role_ID='00A' order by Menu_ID ");
		if(builder.SQL_Error_Code != 0){
			throw new Exception("初始化菜单树列表失败，原因："+builder.SQL_Error_Code+","+builder.SQL_Error_Message);
		}else{
			if(list.size()>0){
				for(int i=0;i<list.size();i++){
					Map<String,Object> map_item = list.get(i);
					String item_id = (String) map_item.get("MENU_ID");
					String item_label = (String) map_item.get("MENU_NAME");
					String item_level = (String) map_item.get("TOP_LEVEL");
					String item_data= (String) map_item.get("PROGRAM_NAME");
					
					Element element_child = new Element("node");
					element_child.setAttribute("id", item_id);
					element_child.setAttribute("data", item_data);
					element_child.setAttribute("label", item_label);
					element_child.setAttribute("top", item_level);
					element_child.setAttribute("type", "0");
					if(list_equal.contains(item_data)){
						element_child.setAttribute("isSelect", "1");
					}else{
						element_child.setAttribute("isSelect", "0");
					}					
					element.addContent(element_child);					
					this.adapterSelectedRoleTree(element_child,item_data, Integer.parseInt(item_level)+1,list_equal);
				}
			}
		}		
	}
	
	public String getInitRoleXMLString(String menu_id,int menu_top_id,String role_id){
		String xml_str = null;
		try{
			this.element = xmlbuilder.getRootElement();
			String sql = "select Menu_Name from T_Role_Menu where Menu_ID=-1 and Top_Level=0 and Role_ID='"+role_id+"'";
			Map<String,Object> map_root = builder.queryMap(sql);
			if(builder.SQL_Error_Code != 0){
				throw new Exception("初始化菜单树列表失败，原因："+builder.SQL_Error_Code+","+builder.SQL_Error_Message);
			}else{
				String menu_name = (String) map_root.get("MENU_NAME");
				this.element.setAttribute("id", menu_id);
				this.element.setAttribute("data", "Happy");
				this.element.setAttribute("label", menu_name);
				this.element.setAttribute("top", menu_top_id+"");
				this.element.setAttribute("isSelect", "0");
				this.element.setAttribute("type", "0");
				
				this.adapterRoleTree(this.element, menu_id, menu_top_id+1,role_id);
			}			
			
			xml_str =  xmlbuilder.toXML(this.element);
		}catch(Exception e){
			logger.error(e.getMessage());
		}
		return xml_str;
	}
	
	private void adapterRoleTree(Element element, String parent_id,int top_id,String role_id)throws Exception{		
		List<Map<String,Object>> list = builder.queryList("select Menu_ID,Menu_Name,Top_Level,Program_Name from T_Role_Menu where Parent_ID="+parent_id+" and Top_Level="+top_id+" and Role_ID='"+role_id+"' order by Menu_ID ");
		if(builder.SQL_Error_Code != 0){
			throw new Exception("初始化菜单树列表失败，原因："+builder.SQL_Error_Code+","+builder.SQL_Error_Message);
		}else{
			if(list.size()>0){
				for(int i=0;i<list.size();i++){
					Map<String,Object> map_item = list.get(i);
					String item_id = (String) map_item.get("MENU_ID");
					String item_name = (String) map_item.get("MENU_NAME");
					String item_level = (String) map_item.get("TOP_LEVEL");
					String item_data = (String) map_item.get("PROGRAM_NAME");
					
					Element element_child = new Element("node");
					element_child.setAttribute("id", item_id);
					element_child.setAttribute("data", item_data);
					element_child.setAttribute("label", item_name);
					element_child.setAttribute("top", item_level);
					element_child.setAttribute("isSelect", "0");
					element_child.setAttribute("type", "0");
					element.addContent(element_child);
					
					this.adapterRoleTree(element_child,item_id, Integer.parseInt(item_level)+1,role_id);
				}
			}
		}		
	}
	
	public String getInitRoleMenu(String role_id,String role_name,String type){
		String xml = null;
		Element element = new Element("node");
		element.setAttribute("id", "-1");
		element.setAttribute("data", "Happy");
		element.setAttribute("label", role_name);
		element.setAttribute("top", "0");
		element.setAttribute("isSelect", "1");

		List<String> list_init = builder.queryListForSingleField("Menu_ID", "T_Role_Menu", "Role_ID='"+role_id+"' and TOP_LEVEL=1");		
		if(list_init.size()>0){			
			for(int k=0;k<list_init.size();k++){
				String top_menu_title_id = list_init.get(k);
				Element element_item =  this.getSingleDepartmentXML(role_id, Integer.parseInt(top_menu_title_id),"0",null);
				element.addContent(element_item);
			}
			XMLBuilder xb = new XMLBuilder();
			xml = xb.toXML(element);
		}		
		return xml;
	}
	
	public Element getSingleDepartmentXML(String role_id,int menu_id,String isSelect,List<String> list_role_equal){
		String xml = null;
		Element element = new Element("node");
		String sql_map = "select Menu_ID,Menu_Name,Top_Level,Program_Name from T_Role_Menu where Menu_ID="+menu_id+" and Role_ID='00A' and Top_Level=1";
		Map<String,Object> map = builder.queryMap(sql_map);
		if(map != null){
			String id = (String) map.get("MENU_ID");
			String label = (String) map.get("MENU_NAME");
			String top = (String) map.get("TOP_LEVEL");
			String data = (String) map.get("PROGRAM_NAME");
			element.setAttribute("id", id);
			element.setAttribute("data", data);
			element.setAttribute("label", label);
			element.setAttribute("top", top);
			element.setAttribute("isSelect", isSelect);
			
			String sql = "select Menu_ID,Menu_Name,Top_Level,Program_Name from T_Role_Menu where Role_ID='00A' and Top_Level=2 and Parent_ID="+menu_id+"";
			List<Map<String,Object>> list = builder.queryList(sql);
			for(int i=0;i<list.size();i++){
				Map<String,Object> map_item = list.get(i);
				String id_item = (String) map_item.get("MENU_ID");
				String label_item = (String) map_item.get("MENU_NAME");
				String top_item = (String) map_item.get("TOP_LEVEL");
				String data_item = (String) map_item.get("PROGRAM_NAME");
				String isSelect_item = "0";
				if(list_role_equal != null){
					if(list_role_equal.contains(id_item)){
						isSelect_item = "1";
					}
				}
				
				Element elem_item = new Element("node");
				elem_item.setAttribute("id", id_item);
				elem_item.setAttribute("data", data_item);
				elem_item.setAttribute("label", label_item);
				elem_item.setAttribute("top", top_item);
				elem_item.setAttribute("isSelect", isSelect_item);
				elem_item.setAttribute("type", "0");
				element.addContent(elem_item);
			}			
		}
		return element;
	}
	
	public static void main(String[] args){
	
	}
}
