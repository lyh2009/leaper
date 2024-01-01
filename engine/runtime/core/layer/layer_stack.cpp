#include "layer_stack.h"

Leaper::LayerStack::~LayerStack()
{
    for(Layer* layer : m_layers)
    {
        layer->OnDetach();
        delete layer;
    }
}

void Leaper::LayerStack::PushLayer(Layer* layer)
{
    m_layers.emplace(m_layers.begin() + m_layer_insert_index, layer);
    m_layer_insert_index++;
}

void Leaper::LayerStack::PushOverlay(Layer* overlay)
{
    m_layers.push_back(overlay);
}

void Leaper::LayerStack::PopLayer(Layer* layer)
{
    auto item = std::find(m_layers.begin(), m_layers.begin() + m_layer_insert_index, layer);

    if(item != m_layers.begin() + m_layer_insert_index)
    {
        layer->OnDetach();
        m_layers.erase(item);
        delete layer;
        m_layer_insert_index--;
    }
}

void Leaper::LayerStack::PopOverlay(Layer* overlay)
{
    auto item = std::find(m_layers.begin(), m_layers.end(), overlay);
    if(item != m_layers.end())
    {
        overlay->OnDetach();
        m_layers.erase(item);
    }
}