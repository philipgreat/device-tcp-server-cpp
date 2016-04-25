package cn.ucans.tree
{
	import flash.xml.XMLNode;
	
	public class TreeNode
	{	
		private var xml:XML = null;
		private var xmlString:String = null;
		public var name:String = "node";
		public var top:int = 0;
		public var id:String = "";
		public var data:String = "";
		public var label:String = "";
		public var isSelect:int = 0;
		public var type:int = 0;
		public var childable:int = 0;
		public var childrenNode:String = "";
		
		public function TreeNode()
		{
		}
		
		public function toString():String{
			this.xmlString = "<"+this.name+" id=\""+this.id+"\" data=\""+this.data+"\" label=\""+this.label+"\" top=\""+this.top+"\" isSelect=\""+this.isSelect+"\" type=\""+this.type+"\" />";
			//this.xml = new XML(this.xmlString);	
			return this.xmlString;
		} 
		
		public function toXML():XML{
			this.xml = <node/>;
			//this.xml.setName(this.name);
			this.xml.@label = this.label;
			this.xml.@data = this.data;
			this.xml.@top = ""+this.top;
			this.xml.@isSelect = ""+this.isSelect;
			this.xml.@type = ""+this.type;
			this.xml.@id = ""+this.id;
			return this.xml;
		}
	}
}