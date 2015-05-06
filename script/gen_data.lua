header = {
    { name='length', type='uint16_t' },
    { name='type', type='uint16_t' }
}

login = {
    { name='user_id', type='char', length="16" }
}

order_parameters = {
    { name='symbol', type='char', length='8' },
    { name='type', type='order_type_t' },
    { name='side', type='order_side_t' },
    { name='quantity', type='order_quantity_t' },
    { name='price', type='order_price_t' }
}

place_order = {
    { name='user_id', type='exchange_id_t' },
    { name='transaction_id', type='exchange_id_t' },
    { name='parameters', type='order_parameters' }
}

pull_order = {
    { name='user_id', type='exchange_id_t' },
    { name='transaction_id', type='exchange_id_t' }
}

order_rejected = {
    { name='user_id', type='exchange_id_t' },
    { name='transaction_id', type='exchange_id_t' },
    { name='parameters', type='order_parameters' }
}

order_placed = {
    { name='user_id', type='exchange_id_t' },
    { name='transaction_id', type='exchange_id_t' },
    { name='parameters', type='order_parameters' }
}

order_pulled = {
    { name='user_id', type='exchange_id_t' },
    { name='transaction_id', type='exchange_id_t' },
    { name='leaves', type='order_quantity_t' },
    { name='parameters', type='order_parameters' }
}

order_executed = {
    { name='user_id', type='exchange_id_t' },
    { name='transaction_id', type='exchange_id_t' },
    { name='exec_price', type='order_price_t' },
    { name='exec_quantity', type='order_quantity_t' },
    { name='leaves', type='order_quantity_t' },
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
