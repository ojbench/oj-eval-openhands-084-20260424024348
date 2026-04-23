
#include <iostream>
#include <cassert>
#include "src.hpp"

using std::cout, std::endl;

int main() {
    // Test 1: Basic insert and ask with proper sequence
    {
        LinkedHashMap map;
        map.init(13, true);  // len=13, traverse by time
        map.insert(3, "7BQEzo8knH24pxK9hOqm");
        map.remove(3, "7BQEzo8knH24pxK9hOqm");
        map.insert(6, "zr9TUZ3LAPytFkgsJdYx");
        
        // Test ask - should return only "zr9TUZ3LAPytFkgsJdYx"
        vector<string> result = map.ask(6);
        cout << "Result size: " << result.size() << endl;
        if (result.size() > 0) {
            cout << "First value: '" << result[0] << "'" << endl;
            cout << "Expected: 'zr9TUZ3LAPytFkgsJdYx'" << endl;
        }
        assert(result.size() == 1);
        assert(result[0] == "zr9TUZ3LAPytFkgsJdYx");
        
        // Now insert another value for key 6
        map.insert(6, "xnXaD6tRGdQEM8oSghKJ");
        result = map.ask(6);
        cout << "After second insert, result size: " << result.size() << endl;
        if (result.size() > 0) {
            cout << "First value: '" << result[0] << "'" << endl;
            cout << "Second value: '" << result[1] << "'" << endl;
        }
        assert(result.size() == 2);
        assert(result[0] == "xnXaD6tRGdQEM8oSghKJ");
        assert(result[1] == "zr9TUZ3LAPytFkgsJdYx");
        
        // Test remove
        map.remove(6, "zr9TUZ3LAPytFkgsJdYx");
        result = map.ask(6);
        cout << "After remove, result size: " << result.size() << endl;
        if (result.size() > 0) {
            cout << "First value: '" << result[0] << "'" << endl;
        }
        assert(result.size() == 1);
        assert(result[0] == "xnXaD6tRGdQEM8oSghKJ");
        
        // Test remove non-existent
        map.remove(99, "nonexistent");
        result = map.ask(99);
        assert(result.size() == 0);
        
        map.clearMemory();
        cout << "Test 1 passed!" << endl;
    }
    
    // Test 2: Time-based traversal
    {
        LinkedHashMap map;
        map.init(13, true);  // traverse by time
        map.insert(3, "A");
        map.insert(6, "B");
        map.insert(10, "C");
        map.insert(7, "D");
        map.insert(6, "E");
        
        auto result = map.forEach();
        cout << "Time-based traversal result size: " << result.size() << endl;
        for (int i = 0; i < result.size(); i++) {
            cout << "result[" << i << "]: key=" << result[i].key << ", value=" << result[i].value << endl;
        }
        
        assert(result.size() == 5);
        assert(result[0].key == 3 && result[0].value == "A");
        assert(result[1].key == 6 && result[1].value == "B");
        assert(result[2].key == 10 && result[2].value == "C");
        assert(result[3].key == 7 && result[3].value == "D");
        assert(result[4].key == 6 && result[4].value == "E");
        
        map.clearMemory();
        cout << "Test 2 passed!" << endl;
    }
    
    // Test 3: Index-based traversal
    {
        LinkedHashMap map;
        map.init(8, false);  // traverse by index
        map.insert(3, "A");
        map.insert(0, "B");
        map.insert(5, "C");
        map.insert(7, "D");
        map.insert(2, "E");
        map.insert(5, "F");
        
        auto result = map.forEach();
        cout << "Index-based traversal result size: " << result.size() << endl;
        for (int i = 0; i < result.size(); i++) {
            cout << "result[" << i << "]: key=" << result[i].key << ", value=" << result[i].value << endl;
        }
        
        assert(result.size() == 6);
        // Should be ordered by index, and within each index, from head to tail (most recent first)
        assert(result[0].key == 0 && result[0].value == "B");
        assert(result[1].key == 2 && result[1].value == "E");
        assert(result[2].key == 3 && result[2].value == "A");
        assert(result[3].key == 5 && result[3].value == "F");  // Most recent for key 5
        assert(result[4].key == 5 && result[4].value == "C");  // Least recent for key 5
        assert(result[5].key == 7 && result[5].value == "D");
        
        map.clearMemory();
        cout << "Test 3 passed!" << endl;
    }
    
    // Test 4: Remove from middle of time list
    {
        LinkedHashMap map;
        map.init(13, true);
        map.insert(1, "A");
        map.insert(2, "B");
        map.insert(3, "C");
        
        // Remove middle element
        map.remove(2, "B");
        auto result = map.forEach();
        assert(result.size() == 2);
        assert(result[0].key == 1 && result[0].value == "A");
        assert(result[1].key == 3 && result[1].value == "C");
        
        map.clearMemory();
        cout << "Test 4 passed!" << endl;
    }
    
    // Test 5: Remove from head of time list
    {
        LinkedHashMap map;
        map.init(13, true);
        map.insert(1, "A");
        map.insert(2, "B");
        map.insert(3, "C");
        
        // Remove head element
        map.remove(1, "A");
        auto result = map.forEach();
        assert(result.size() == 2);
        assert(result[0].key == 2 && result[0].value == "B");
        assert(result[1].key == 3 && result[1].value == "C");
        
        map.clearMemory();
        cout << "Test 5 passed!" << endl;
    }
    
    // Test 6: Remove from tail of time list
    {
        LinkedHashMap map;
        map.init(13, true);
        map.insert(1, "A");
        map.insert(2, "B");
        map.insert(3, "C");
        
        // Remove tail element
        map.remove(3, "C");
        auto result = map.forEach();
        assert(result.size() == 2);
        assert(result[0].key == 1 && result[0].value == "A");
        assert(result[1].key == 2 && result[1].value == "B");
        
        map.clearMemory();
        cout << "Test 6 passed!" << endl;
    }
    
    // Test 7: Multiple removes
    {
        LinkedHashMap map;
        map.init(13, true);
        map.insert(1, "A");
        map.insert(1, "B");
        map.insert(1, "C");
        
        // Remove all from same key
        map.remove(1, "A");
        map.remove(1, "B");
        map.remove(1, "C");
        auto result1 = map.ask(1);
        assert(result1.size() == 0);
        auto result2 = map.forEach();
        assert(result2.size() == 0);
        
        map.clearMemory();
        cout << "Test 7 passed!" << endl;
    }
    
    cout << "All tests passed!" << endl;
    return 0;
}
