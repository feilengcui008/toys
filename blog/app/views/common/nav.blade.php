<div class="blog-masthead">
	<div class="container">
		<nav class="blog-nav">
			<a class="blog-nav-item {{ (Request::is('/') ? ' active' : '') }}" href="/">首页</a>
			
			<a class="blog-nav-item {{ (Request::is('about') ? ' active' : '') }}" href="/about">关于</a>
			<span style="float:right">
			<a class="blog-nav-item">
				@if(Sentry::check())
				{{Sentry::getUser()->first_name}}
				_{{Sentry::getUser()->last_name}}
				<a href="/admin" class="blog-nav-item">Admin</a>
				<a href="/logout" class="blog-nav-item">logout</a>
				@endif
			</a>
			</span>
		</nav>
	</div>
</div>
