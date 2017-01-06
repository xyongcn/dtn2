
class BundleConfig
{

	//较少副本转发的副本上限
public:
	static const int DELIVERBUNDLENUM=2;

	//受限洪泛算法的副本上限
	static const int FLOODBUNDLENUM=2;


	/**
	 * Default expiration time in seconds, set to 1 hour
	 */
	static const int EXPIRATION_TIME = 1*60*60;

	/**
	 * Set delivery options to don't flag at all
	 */
	static const int DELIVERY_OPTIONS = 0;

	/**
	 * Set priority to normal sending
	 */
	//static const dtn_bundle_priority_t PRIORITY = dtn_bundle_priority_t::COS_NORMAL;
};
