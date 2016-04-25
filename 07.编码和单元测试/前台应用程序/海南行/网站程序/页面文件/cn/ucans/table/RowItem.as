package cn.ucans.table
{
	import cn.ucans.common.RegularExpression;
	
	public class RowItem
	{	
		private var xml:XML = null;
		private var regular:RegularExpression = new RegularExpression();
		public function RowItem()
		{
			this.xml = new XML("<row></row>")
		}
		
		public function setValue(key:String,value:String):void{
			var item:XML = new XML("<item key=\""+key+"\">"+regular.trim(value)+"</item>");
			this.xml.appendChild(item);
		}
		
		public function toXML():XML{
			return this.xml;
		}
	}
}