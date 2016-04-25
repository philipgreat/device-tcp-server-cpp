package cn.ucans.flex
{	
	import mx.collections.ArrayCollection;
	
	[RemoteClass(alias="cn.ucans.flex.FlexReponse")]
	public class FlexReponse
	{	
		public var sign:int = -1;
		public var message:String = "";
		
		public var ope_id:int = -1;
		public var menu_id:String = "";
		public var org_id:String = "";
		public var user_id:String = "";
		
		public var result:Object = null;
		
		public function FlexReponse()
		{
		}
	}
}