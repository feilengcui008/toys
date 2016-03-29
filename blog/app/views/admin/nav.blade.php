<div class="blog-masthead">
	<div class="container">
		<nav class="blog-nav">
			<a class="blog-nav-item {{ (Request::is('/') ? ' active' : '') }}" href="/">首页</a>
			<a class="blog-nav-item {{ (Request::is('/admin') ? ' active' : '') }}" href="/admin/">文章管理</a>
			<a class="blog-nav-item {{ (Request::is('/admin/tag') ? ' active' : '') }}" href="/admin/tag">标签管理</a>
			<a class="blog-nav-item {{ (Request::is('/manage') ? ' active' : '') }}" href="/manage">进入laravel-administrator后台</a>
			<span style="float:right">
			<a class="blog-nav-item">
				@if(Sentry::check())
				{{Sentry::getUser()->first_name}}
				_{{Sentry::getUser()->last_name}}
				<a href="/logout" class="blog-nav-item">logout</a>
				@endif
			</a>
			</span>
		</nav>
	</div>
</div>
