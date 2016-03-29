<a id="backtop" href="javascript:void" style="display: inline;"></a>
<div class="blog-footer">
    <p>made by <a href="https://github.com/feilengcui008">feilengcui008</a></p>
</div>
<script src="/js/jquery.js" ></script>
<script src="/js/jquery.pjax.js"></script>
<script src='/js/nprogress.js'></script>
<script type="text/javascript">
$(document).ready(function () {


//$(document).pjax('a', 'body');
$(document).on('pjax:start', function() {
  NProgress.start();
  });
$(document).on('pjax:end', function() {
  NProgress.done();
  self.siteBootUp();
  });

//首先将#back-to-top隐藏
$("#backtop").hide();
//当滚动条的位置处于距顶部100像素以下时，跳转链接出现，否则消失
$(function () {
	
$(window).scroll(function () {
if ($(window).scrollTop() > 90) {
$("#backtop").fadeIn(1000);
}
else {
$("#backtop").fadeOut(1000);
}
});
//当点击跳转链接后，回到页面顶部位置
$("#backtop").click(function () {
$('body,html').animate({ scrollTop: 0 }, 100);
return false;
});


});


});


</script>
<script>
    function reply(id,uname)
    {
        $('.reply').remove();
        at = "<p class='reply'>@<span class='text-info'>"+uname+"</span>:</p>";
        atinput = "<p class='reply'><input type='hidden' name='at' value='"+uname+"'/></p>";
        $('#replyform').prepend(at);
        $('#replyform').prepend(atinput);
        window.scrollTo(0,99999);
    }
</script>
