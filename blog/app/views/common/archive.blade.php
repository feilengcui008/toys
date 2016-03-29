<div class="sidebar-module">
	<h4><strong>点击量排行</strong></h4>
	<ul class="nav nav-pills nav-stacked">
		@if($click_articles)
		@foreach($click_articles as $article)
		<li><a href="{{URL::route('article.show',array($article->id))}}">{{$article->title}}</a></li>
		@endforeach
		@endif
	</ul>
</div>