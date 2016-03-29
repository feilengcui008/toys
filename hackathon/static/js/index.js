$(document).ready(function(){
	if(document.referrer!="")
	{
		$("#welcomePage").hide();
	}
	$("#hide").click(function() {
		$("#welcomePage").hide();
	});
	$(".image").click(function(){
		$("#myModalLabel").text($(this).attr("data-name"));
		$("#backgroundImg").val($(this).attr("id"));
		$('#myModal').modal('show');
		$("#file").change(function() {
			$("#previewLabel").val("图片预览")
			var url = getObjectURL(this.files[0]) ;
			var image = '<image class="imgIndex" src="'+url+'"></image>';
		$("#priview").html(image);

		});
        });

})
function getObjectURL(file) {
	var url = null ; 
	if (window.createObjectURL!=undefined) { // basic
		url = window.createObjectURL(file) ;
	} else if (window.URL!=undefined) { // mozilla(firefox)
		url = window.URL.createObjectURL(file) ;
	} else if (window.webkitURL!=undefined) { // webkit or chrome
		url = window.webkitURL.createObjectURL(file) ;
	}
	return url ;
}