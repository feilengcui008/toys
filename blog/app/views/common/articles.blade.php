<div class="col-sm-8 blog-main">
	@if(count($articles)>0)
	@foreach($articles as $article)
	<hr />
	<div class="blog-post">
		<div class="blog-post-title">
			<a href="{{URL::route('article.show',array($article->id))}}">{{$article->title}}</a>
			<blockquote><small><cite title="Source Title">{{$article->description}}</cite></small></blockquote>
		</div>
		<p>
		<span class="blog-post-meta">{{date("Y/m/d",strtotime($article->updated_at))}}</span>
		<span class="glyphicon glyphicon-hand-down btn-sm">{{$article->click_num}}</span>
		</p>


                           <!-- get the tags for each article -->
		<p class="blog-post-meta">
		@if($blogtagarray)
		    @foreach($blogtagarray as $key => $blogtag)
		        @if($key == $article->id)
		        @foreach($blogtag as $val)
		            <a href="/tag/{{$val->id}}">
		            <button type="button" class="btn btn-default btn-sm tag">
			<span class="glyphicon glyphicon-tag"></span> {{$val->name}}
			</button></a>
		        @endforeach
		         @endif
		    @endforeach
		@endif
		</p>


		<p>
		@if(strlen($article->content) > 200)
		{{ mb_substr($article->content,0,200,'utf-8') }}...<br />
		<p align="right"><a href="/article/{{$article->id}}"><button class="btn btn-default btn-lg morebtn">more</button></a></p>
		@else
		{{$article->content}}
		@endif
		</p>
	</div>
	@endforeach
	<ul class="pager">
		{{ $articles->links() }}
	</ul>
	@else
	<hr />
	<p>there has nothing yet</p>
	@endif
	
	
</div>