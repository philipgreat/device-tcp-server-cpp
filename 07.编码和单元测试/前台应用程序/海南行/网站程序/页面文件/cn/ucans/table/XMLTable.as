package cn.ucans.table
{
	public class XMLTable
	{	
		private var xml:XML = null;
		
		public function XMLTable()
		{
			xml = new XML("<table title=\"\"></table>");
		}
		
		public function addRowItem(row:RowItem):void{
			this.xml.appendChild(row.toXML());
		}
		
		public function addTitle(title:String):void{
			this.xml.@title = title;
		}
		
		public function toXMLString():String{
			return this.xml.toXMLString();
		}
	}
}