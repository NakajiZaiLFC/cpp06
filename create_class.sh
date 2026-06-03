#!/bin/bash

CLASS_NAME=$1
UPPER_NAME=$(echo $CLASS_NAME | tr '[:lower:]' '[:upper:]')

# ヘッダーファイル生成
cat > ${CLASS_NAME}.hpp << EOF
#pragma once

class ${CLASS_NAME} {
public:
    ${CLASS_NAME}();
    ${CLASS_NAME}(const ${CLASS_NAME}& other);
    ${CLASS_NAME}& operator=(const ${CLASS_NAME}& other);
    ~${CLASS_NAME}();

private:
    
};

EOF

# 実装ファイル生成
cat > ${CLASS_NAME}.cpp << EOF
#include "${CLASS_NAME}.hpp"

${CLASS_NAME}::${CLASS_NAME}() {
    
}

${CLASS_NAME}::${CLASS_NAME}(const ${CLASS_NAME}& other) {
    *this = other;
}

${CLASS_NAME}& ${CLASS_NAME}::operator=(const ${CLASS_NAME}& other) {
    if (this != &other) {
    }
    return *this;
}

${CLASS_NAME}::~${CLASS_NAME}() {
    
}
EOF

echo "Class ${CLASS_NAME} created successfully!"