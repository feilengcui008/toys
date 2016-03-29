<!DOCTYPE html>
<html lang="en">
	<head>
		@include('common.header')
	</head>
	<body>
		@include('common.nav')
		<div class="container">
			<div class="row">
				@yield('blogs')
				@yield('sidebar')
				@yield('content')
			</div>
		</div>
		
		@include('common.footer')
		
	</body>
</html>