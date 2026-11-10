
#include <memory>
#include <span>
#include <vector>
#include <filesystem>

#include <glm/glm.hpp>

namespace mc_core
{
	// TODO
	
	// sphere volume debug function 
	
	// define tables / data structs in a separate file

	// implement CPU extraction (separate file)

	// use library to write mesh to output file?


	class Volume
	{
	public:
		explicit Volume(const std::filesystem::path& loadPath);

		glm::uvec3 getDims() const { return dims; }
		uint32_t getChannels() const { return channels; }
		uint32_t getBitsPerChannel() const { return bitsPerChannel; }

		// span returns a pointer plus a size
		// const in the type specifier indicates a read only uint8_t view
		std::span<const uint8_t> getData() const { return data; }

		float sample(glm::uvec3 p, uint32_t channel = 0) const;

	private:
		void parse_volume_properties_from_filename(const std::string& filename);
		void load_volume(const std::filesystem::path& filename);

	private:
		glm::uvec3 dims{};
		uint32_t channels{};
		uint32_t bitsPerChannel{};

		std::vector<uint8_t> data;
	};

}
