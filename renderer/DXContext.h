#pragma once


namespace xwf {

	class DXContext {

	public:
		bool Init();
		void Shutdown();

	private:


	public:
		DXContext(const DXContext&) = delete;
		DXContext& operator=(const DXContext&) = delete;

		inline static DXContext& Get()
		{
			static DXContext instance;
			return instance;
		}

	private:
		DXContext() = default;
	};
}