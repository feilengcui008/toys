@include('common.footer')
<script type="text/javascript" src="/editor/js/simditor-all.js"></script>
<script src="/editor/js/simditor-emoji.js"></script>
<script src="/editor/js/simditor-markdown.js"></script>
<script>
    $(document).ready(function(){
		var editor = new Simditor({
		textarea: $('#textdomain'),
		markdown:true,
		placeholder: '这里输入文字...',
		toolbar: ['title', 'bold', 'italic', 'underline', 'strikethrough', 'color', '|', 'ol', 'ul', 'blockquote', 'code',  '|', 'link', 'image', 'hr', '|', 'indent', 'outdent','emoji'],
		mobileToolbar: ["bold", "underline", "strikethrough", "color", "ul", "ol"],
		emoji: {
				imagePath: '/editor/img/emoji/'
			}
  
});

});
		
</script>
