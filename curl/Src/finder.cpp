#include "../Sources/finder.h"
std::vector<int> Finder::friends(int user_id) {
    std::ostringstream buf;
    buf << "https://api.vk.com/method/friends.get?user_id=" << user_id;
    std::string result( req.run(buf.str()) );


    std::vector<int> r;
    if(result.length()) {

	    json::Object obj(json::Deserialize(result));
	    json::Value response(obj["response"]);

	    if (response.GetType() == json::ArrayVal) {
	        json::Array ids(response);
	        for (
	             std::vector< json::Value >::const_iterator iter = ids.begin(), end = ids.end();
	             iter != end; ++iter
	             ) {
	            if (iter->IsNumeric()) {
	                r.push_back(*iter);
	            }
	        }
	    }
    }
    return r;
}