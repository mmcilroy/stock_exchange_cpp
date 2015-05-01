header = {
    { name='length', type='uint16_t' },
    { name='type', type='uint16_t' }
}

login = {
    { name='user_id', type='uint32_t' }
}

order_parameters = {
    { name='type', type='uint16_t' },
    { name='side', type='uint16_t' },
    { name='quantity', type='uint32_t' },
    { name='price', type='uint32_t' }
}

place_order = {
    { name='user_id', type='uint32_t' },
    { name='transaction_id', type='uint32_t' },
    { name='parameters', type='order_parameters' }
}

pull_order = {
    { name='user_id', type='uint32_t' },
    { name='transaction_id', type='uint32_t' }
}

order_rejected = {
    { name='user_id', type='uint32_t' },
    { name='transaction_id', type='uint32_t' },
    { name='parameters', type='order_parameters' }
}

order_placed = {
    { name='user_id', type='uint32_t' },
    { name='transaction_id', type='uint32_t' },
    { name='parameters', type='order_parameters' }
}

order_pulled = {
    { name='user_id', type='uint32_t' },
    { name='transaction_id', type='uint32_t' },
    { name='leaves_quantity', type='uint32_t' },
    { name='parameters', type='order_parameters' }
}

order_executed = {
    { name='user_id', type='uint32_t' },
    { name='transaction_id', type='uint32_t' },
    { name='exec_price', type='uint32_t' },
    { name='exec_quantity', type='uint32_t' },
    { name='leaves_quantity', type='uint32_t' },
    { name='parameters', type='order_parameters' }
}

names = {
    'header',
    'login',
    'order_parameters',
    'order_rejected',
    'order_placed',
    'order_pulled',
    'order_executed',
    'place_order',
    'pull_order'
}
